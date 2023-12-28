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

    void SetWorldMatrix(float* p)
    {
        if (p == nullptr)
        {
            return;
        }

        DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(p));
        DirectX::XMVECTOR scaleVec, rotQuat, transVec;
        DirectX::XMMatrixDecompose(&scaleVec, &rotQuat, &transVec, worldMatrix);

        DirectX::XMFLOAT3 scale{};
        DirectX::XMFLOAT3 translation{};
        DirectX::XMStoreFloat3(&scale, scaleVec);
        DirectX::XMStoreFloat3(&translation, transVec);

        DirectX::XMFLOAT3 rotation{};
        DirectX::XMVECTOR rotEuler = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotQuat);
        DirectX::XMStoreFloat3(&rotation, rotEuler);

        Position = Vec3(translation.x, translation.y, translation.z);
        Rotation = Vec3(DirectX::XMConvertToDegrees(rotation.x),
                        DirectX::XMConvertToDegrees(rotation.y),
                        DirectX::XMConvertToDegrees(rotation.z));
        Scale = Vec3(scale.x, scale.y, scale.z);
    }

    Vec3 Position = {};
    Vec3 Rotation = {};
    Vec3 Scale = {};
};
