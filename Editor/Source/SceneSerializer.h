﻿#pragma once
#include <json.hpp>
#include <memory>

#include "SpotLightComponent.h"
#include "Engine/Implementation/MeshComponent.h"

class PointLightComponent;
class DirectionalLightComponent;
class SpriteComponent;
class CameraComponent;
class OrthographicCamera;
class PerspectiveCamera;
struct Transform3D;
class GameObject;
class Scene;

class SceneSerializer
{
public:
    SceneSerializer(IGraphics* graphics);
    static void WriteToFile(nlohmann::json sceneData, std::string path);
    static nlohmann::json SerializeMaterial(const std::shared_ptr<IRenderableComponent>& meshComponent,
                                            nlohmann::json serializedData);
    static nlohmann::json SerializeMeshComponent(const std::shared_ptr<MeshComponent>& shared);
    static nlohmann::json SerializeSpriteComponent(const std::shared_ptr<SpriteComponent>& shared);

    static nlohmann::json SerializeSpotLight(const std::shared_ptr<SpotLightComponent>& shared);
    static nlohmann::json SerializePointLight(const std::shared_ptr<PointLightComponent>& shared);
    static nlohmann::json SerializeGameObject(const std::shared_ptr<GameObject>& object);
    static nlohmann::json SerializeTransform(std::shared_ptr<Transform3D> transform);
    static nlohmann::json SerializeCameraComponent(std::shared_ptr<CameraComponent> camera);
    static nlohmann::json SerializeOrthographicCamera(std::shared_ptr<OrthographicCamera> camera);
    static nlohmann::json SerializePerspectiveCamera(std::shared_ptr<PerspectiveCamera> camera);
    static nlohmann::json SerializeDirectionalLight(std::shared_ptr<DirectionalLightComponent> dirLight);

    static std::shared_ptr<Scene> Deserialize(std::string path);
    static IMaterial* DeserializeMaterial(const nlohmann::json& data, std::string texturePath, std::string shaderPath);

    static void DeserializeMeshComponent(const std::shared_ptr<GameObject>& shared, const nlohmann::json& value);
    static void DeserializeSpriteComponent(const std::shared_ptr<GameObject>& shared, const nlohmann::json& value);

    static void DeserializeDirectionalLight(const std::shared_ptr<GameObject>& gameObject, const nlohmann::json& data,
                                            std::shared_ptr<Scene> scene);
    static void DeserializePointLight(const std::shared_ptr<GameObject>& gameObject, const nlohmann::json& data,
                               std::shared_ptr<Scene> scene);
    static void DeserializeSpotLight(const std::shared_ptr<GameObject>& gameObject, const nlohmann::json& data,
                               std::shared_ptr<Scene> scene);
    static std::shared_ptr<GameObject> DeserializeGameObject(const nlohmann::json& data,
                                                             std::unordered_map<std::string, std::string>& parentsDict, std::shared_ptr<Scene> scene);
    static std::shared_ptr<Transform3D> DeserializeTransform(const nlohmann::json& data, std::string& parentGuid);
    static std::shared_ptr<CameraComponent> DeserializeCameraComponent(std::shared_ptr<GameObject> gameObject,
                                                                       const nlohmann::json& data);
    static std::shared_ptr<OrthographicCamera> DeserializeOrthographicCamera(const nlohmann::json& data);
    static std::shared_ptr<PerspectiveCamera> DeserializePerspectiveCamera(const nlohmann::json& data);
    static bool Serialize(
        std::string path = "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Scenes/TestScene.scene");

private:
    inline static IGraphics* graphics;
};
