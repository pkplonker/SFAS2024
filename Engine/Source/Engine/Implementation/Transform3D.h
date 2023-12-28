#pragma once
#include "Transform.h"
#include "Engine/Math/Vector3.h"
#include <DirectXMath.h>

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
        DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(Position.X(), Position.Y(), Position.Z());
        DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(Rotation.X()),
            DirectX::XMConvertToRadians(Rotation.Y()),
            DirectX::XMConvertToRadians(Rotation.Z()));
        DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(Scale.X(), Scale.Y(), Scale.Z());
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

    Vec3 Position = {};
    Vec3 Rotation = {};
    Vec3 Scale = {};
};
