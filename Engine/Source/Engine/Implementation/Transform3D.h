#pragma once
#include "Transform.h"
#include "Engine/Math/Vector3.h"
#include <DirectXMath.h>
#include <iostream>

#include "xdsp.h"

struct Transform3D : Transform
{
    Transform3D() : Transform3D(Vec3::Zero(), Vec3::Zero(), Vec3::One())
    {
    }

    Transform3D(Vec3 position, Vec3 rotation, Vec3 scale)
        : Position(position), Rotation(rotation), Scale(scale)
    {
    }

    Transform3D(Vec3 position, Vec3 rotation)
        : Transform3D(position, rotation, Vec3::One())
    {
    }

    Transform3D(Vec3 position)
        : Transform3D(position, Vec3::Zero(), Vec3::One())
    {
    }

    DirectX::XMMATRIX GetWorldMatrix()
    {
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(Scale.X(), Scale.Y(), Scale.Z());
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(Rotation.X()),
            DirectX::XMConvertToRadians(Rotation.Y()),
            DirectX::XMConvertToRadians(Rotation.Z()));
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(Position.X(), Position.Y(), Position.Z());

        return scaleMatrix * rotationMatrix * translation;
    }

    DirectX::XMMATRIX GetWorldMatrixEuler()
    {
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(Scale.X(), Scale.Y(), Scale.Z());
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
            Rotation.X(),
            Rotation.Y(),
            Rotation.Z());
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(Position.X(), Position.Y(), Position.Z());

        return scaleMatrix * rotationMatrix * translation;
    }

    void SetTranslation(float* data)
    {
        Position = Vec3(data);
    }

    void SetRotationQuat(float* data)
    {
        DirectX::XMVECTOR quat = DirectX::XMLoadFloat4(reinterpret_cast<const DirectX::XMFLOAT4*>(data));

        DirectX::XMFLOAT3 euler;
        DirectX::XMVECTOR rotationVec = DirectX::XMQuaternionRotationRollPitchYawFromVector(quat);
        DirectX::XMStoreFloat3(&euler, rotationVec);

        euler.x = DirectX::XMConvertToDegrees(euler.x);
        euler.y = DirectX::XMConvertToDegrees(euler.y);
        euler.z = DirectX::XMConvertToDegrees(euler.z);

        // Set Rotation
        Rotation = Vec3(euler.x, euler.y, euler.z);
    }

    void SetRotationEuler(float* data)
    {
        Rotation = Vec3(data);
    }

    void SetScale(float* data)
    {
        Scale = Vec3(data);
    }

    // void SetWorldMatrix(float* p)
    // {
    //     // Assuming p points to a 4x4 matrix in row-major order
    //
    //     // Extract the translation
    //     Position.X(p[12]);
    //     Position.Y(p[13]);
    //     Position.Z(p[14]);
    //
    //     // Extract the scale from the rotation matrix
    //     Scale.X(sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]));
    //     Scale.Y(sqrt(p[4] * p[4] + p[5] * p[5] + p[6] * p[6]));
    //     Scale.Z(sqrt(p[8] * p[8] + p[9] * p[9] + p[10] * p[10]));
    //
    //     // Normalize the rotation part of the matrix
    //     float rotationMatrix[9] = {
    //         p[0] / Scale.X(), p[1] / Scale.X(), p[2] / Scale.X(),
    //         p[4] / Scale.Y(), p[5] / Scale.Y(), p[6] / Scale.Y(),
    //         p[8] / Scale.Z(), p[9] / Scale.Z(), p[10] / Scale.Z()
    //     };
    //
    //     // Extract the rotation (assuming no skew or perspective)
    //     // Convert rotation matrix to Euler angles (in radians)
    //     Rotation.X(atan2(rotationMatrix[7], rotationMatrix[8]));
    //     Rotation.Y(atan2(-rotationMatrix[6], sqrt(rotationMatrix[7] * rotationMatrix[7] + rotationMatrix[8] * rotationMatrix[8])));
    //     Rotation.Z(atan2(rotationMatrix[3], rotationMatrix[0]));
    //
    //     // Convert to degrees if necessary
    //     // Rotation.X(Rotation.X() * 180.0f / M_PI);
    //     // Rotation.Y(Rotation.Y() * 180.0f / M_PI);
    //     // Rotation.Z(Rotation.Z() * 180.0f / M_PI);
    // }

    void SetWorldMatrix(const DirectX::XMMATRIX& matrix)
    {
        // Variables to hold the decomposed values
        DirectX::XMVECTOR scale, rotationQuat, translation;

        // Decompose the matrix
        XMMatrixDecompose(&scale, &rotationQuat, &translation, matrix);

        // Convert translation and scale from XMVECTOR to Vec3
        Position.X(DirectX::XMVectorGetX(translation));
        Position.Y(DirectX::XMVectorGetY(translation));
        Position.Z(DirectX::XMVectorGetZ(translation));

        Scale.X(DirectX::XMVectorGetX(scale));
        Scale.Y(DirectX::XMVectorGetY(scale));
        Scale.Z(DirectX::XMVectorGetZ(scale));

        // Convert rotation quaternion to Euler angles
        float pitch, yaw, roll;
        QuaternionToEuler(rotationQuat, pitch, yaw, roll);

        // Convert radians to degrees and set rotation
        Rotation.X(pitch * 180.0f / DirectX::XM_PI);
        Rotation.Y(yaw * 180.0f / DirectX::XM_PI);
        Rotation.Z(roll * 180.0f / DirectX::XM_PI);
    }

    void QuaternionToEuler(const DirectX::XMVECTOR& q, float& pitch, float& yaw, float& roll)
    {
        DirectX::XMFLOAT4 quat;
        DirectX::XMStoreFloat4(&quat, q);

        float ysqr = quat.y * quat.y;

        float t0 = +2.0f * (quat.w * quat.x + quat.y * quat.z);
        float t1 = +1.0f - 2.0f * (quat.x * quat.x + ysqr);
        roll = atan2(t0, t1);

        float t2 = +2.0f * (quat.w * quat.y - quat.z * quat.x);
        t2 = t2 > 1.0f ? 1.0f : t2;
        t2 = t2 < -1.0f ? -1.0f : t2;
        pitch = asin(t2);

        float t3 = +2.0f * (quat.w * quat.z + quat.x * quat.y);
        float t4 = +1.0f - 2.0f * (ysqr + quat.z * quat.z);
        yaw = atan2(t3, t4);
    }

    void SetWorldMatrix(float* p)
    {
        SetWorldMatrix(DirectX::XMMATRIX(p));
    }

    void Translate(Vec3 vec3)
    {
        Position.X(Position.X() + vec3.X());
        Position.Y(Position.Y() + vec3.Y());
        Position.Z(Position.Z() + vec3.Z());
    }

    void Rotate(Vec3 vec3)
    {
        Rotation.X(Rotation.X() + vec3.X());
        Rotation.Y(Rotation.Y() + vec3.Y());
        Rotation.Z(Rotation.Z() + vec3.Z());
    }

    void ApplyScale(Vec3 vec3)
    {
        Scale.X(Scale.X() + vec3.X());
        Scale.Y(Scale.Y() + vec3.Y());
        Scale.Z(Scale.Z() + vec3.Z());
    }


    Vec3 Position = {};
    Vec3 Rotation = {};
    Vec3 Scale = {};
};
