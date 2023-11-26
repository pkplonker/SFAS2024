﻿#include "SceneSerializer.h"

#include <fstream>
#include <string>
#include "json.hpp"
#include "Engine/IMaterial.h"
#include "Engine/IShader.h"
#include "Engine/ITexture.h"
#include "Engine/ResourceManager.h"
#include "Engine/Implementation/CameraComponent.h"
#include "Engine/Implementation/Debug.h"
#include "Engine/Implementation/GameObjectFactory.h"
#include "Engine/Implementation/Helpers.h"
#include "Engine/Implementation/OrthographicCamera.h"
#include "Engine/Implementation/PerspectiveCamera.h"
#include "Engine/Implementation/Scene.h"
using json = nlohmann::json;

SceneSerializer::SceneSerializer(std::weak_ptr<Scene> scene, IGraphics* graphics)
{
    SceneSerializer::scene = scene;
    SceneSerializer::graphics = graphics;
}


void SceneSerializer::WriteToFile(json sceneData, std::string path)
{
    Debug("Saving to disk")

    std::ofstream outputFile(path);

    if (outputFile.is_open())
    {
        outputFile << sceneData.dump(4);

        outputFile.close();
    }
    else
    {
        Debug("Failed writing to file")
    }
}

nlohmann::json SceneSerializer::SerializeMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    nlohmann::json serializedData;

    serializedData["mesh"] = meshComponent->GetMeshPath();
    if (const auto material = meshComponent->GetMaterial())
    {
        if (const auto shader = material->GetShader())
        {
            serializedData["shader"] = Helpers::WideStringToString(shader->GetPath());
        }
        if (const auto texture = material->GetTexture())
        {
            serializedData["texture"] = Helpers::WideStringToString(texture->GetPath());
        }
    }

    return serializedData;
}

bool SceneSerializer::Serialize(std::string path)
{
    json sceneData;
    json objectsData;
    Debug("Saving")

    if (auto sharedScene = scene.lock())
    {
        for (const auto& object : sharedScene->GetObjects())
        {
            objectsData.push_back(SerializeGameObject(object));
        }

        sceneData["objects"] = objectsData;

        WriteToFile(sceneData, path);
        Debug("Saved")
        return true;
    }
    Debug("Saving failed")

    return false;
}

nlohmann::json SceneSerializer::SerializeGameObject(const std::shared_ptr<GameObject>& object)
{
    json serializedData;

    serializedData["name"] = object->Name;
    serializedData["transform"] = SerializeTransform(object->Transform());

    if (auto cameraComponent = object->GetComponent<CameraComponent>())
    {
        serializedData["cameraComponent"] = SerializeCameraComponent(cameraComponent);
    }
    if (auto meshComponent = object->GetComponent<MeshComponent>())
    {
        serializedData["meshComponent"] = SerializeMeshComponent(meshComponent);
    }

    return std::make_pair(object->Name, serializedData);
}

json SceneSerializer::SerializeTransform(std::shared_ptr<Transform3D> transform)
{
    json serializedData;

    serializedData["position"]["x"] = transform->Position.X();
    serializedData["position"]["y"] = transform->Position.Y();
    serializedData["position"]["z"] = transform->Position.Z();
    serializedData["rotation"]["x"] = transform->Rotation.X();
    serializedData["rotation"]["y"] = transform->Rotation.Y();
    serializedData["rotation"]["z"] = transform->Rotation.Z();
    serializedData["scale"]["x"] = transform->Scale.X();
    serializedData["scale"]["y"] = transform->Scale.Y();
    serializedData["scale"]["z"] = transform->Scale.Z();
    return serializedData;
}

nlohmann::json SceneSerializer::SerializeCameraComponent(std::shared_ptr<CameraComponent> camera)
{
    nlohmann::json serializedData;

    if (auto orthoCamera = std::dynamic_pointer_cast<OrthographicCamera>(camera->GetCamera()))
    {
        nlohmann::json cameraData = SerializeOrthographicCamera(orthoCamera);
        serializedData["camera"] = cameraData;
    }
    else if (auto perspCamera = std::dynamic_pointer_cast<PerspectiveCamera>(camera->GetCamera()))
    {
        nlohmann::json cameraData = SerializePerspectiveCamera(perspCamera);
        serializedData["camera"] = cameraData;
    }

    return serializedData;
}

nlohmann::json SceneSerializer::SerializeOrthographicCamera(std::shared_ptr<OrthographicCamera> camera)
{
    nlohmann::json serializedData;

    serializedData["type"] = "OrthographicCamera";
    serializedData["width"] = camera->GetWidth();
    serializedData["height"] = camera->GetHeight();
    serializedData["near"] = camera->GetNear();
    serializedData["far"] = camera->GetFar();

    return serializedData;
}

nlohmann::json SceneSerializer::SerializePerspectiveCamera(std::shared_ptr<PerspectiveCamera> camera)
{
    nlohmann::json serializedData;

    serializedData["type"] = "PerspectiveCamera";
    serializedData["width"] = camera->GetWidth();
    serializedData["height"] = camera->GetHeight();
    serializedData["near"] = camera->GetNear();
    serializedData["far"] = camera->GetFar();
    serializedData["fov"] = camera->GetFOV();

    return serializedData;
}

std::shared_ptr<Scene> SceneSerializer::Deserialize(std::string path)
{
    json sceneData;

    std::ifstream inputFile(path);
    if (inputFile.is_open())
    {
        try
        {
            inputFile >> sceneData;
            inputFile.close();
        }
        catch (const std::exception& e)
        {
            Debug("Failed to parse scene data: " + std::string(e.what()));
            return nullptr;
        }
    }
    else
    {
        Debug("Failed to open scene file for reading");
        return nullptr;
    }

    std::shared_ptr<Scene> scene = std::make_shared<Scene>(graphics);

    if (sceneData.contains("objects"))
    {
        const json& objectsData = sceneData["objects"];

        if (objectsData.is_array())
        {
            for (const json& objectData : objectsData)
            {
                std::shared_ptr<GameObject> gameObject = DeserializeGameObject(objectData);
                if (gameObject)
                {
                    scene->AddObject(gameObject);
                }
                else
                {
                    Debug("Failed to deserialize GameObject in scene file");
                }
            }
        }
        else
        {
            Debug("Invalid data format for 'objects' field in scene file");
            return nullptr;
        }
    }
    else
    {
        Debug("Missing 'objects' field in scene file");
        return nullptr;
    }

    Debug("Scene deserialization succeded");
    return scene;
}

void SceneSerializer::DeserializeMeshComponent(const std::shared_ptr<GameObject>& gameObject,
                                               const nlohmann::json& data)
{
    auto meshComponent = std::make_shared<MeshComponent>(gameObject);
    std::string texturePath = "";
    std::string shaderPath = "";
    std::string meshPath = "";
    IMaterial* material = nullptr;
    Mesh* mesh = nullptr;
    std::shared_ptr<MeshComponent> component = nullptr;
    if (data.contains("shader"))
    {
        shaderPath = data["shader"];
    }
    if (data.contains("texture"))
    {
        texturePath = data["texture"];
    }
    if (shaderPath != "")
    {
        if (texturePath != "")
        {
            material = ResourceManager::GetMaterial(Helpers::StringToWstring(shaderPath),
                                                    Helpers::StringToWstring(texturePath));
        }
        else
        {
            material = ResourceManager::GetMaterial(Helpers::StringToWstring(shaderPath));
        }
    }

    if (data.contains("mesh"))
    {
        meshPath = data["mesh"];
        mesh = ResourceManager::GetMesh(meshPath);
    }
    if (material != nullptr && mesh != nullptr)
    {
        component = std::make_shared<MeshComponent>(gameObject, graphics->CreateMeshRenderable(material, mesh),
                                                    std::shared_ptr<IMaterial>(material));
    }

    if (component != nullptr)
    {
        gameObject->AddComponent(std::move(component));
    }
}


std::shared_ptr<GameObject> SceneSerializer::DeserializeGameObject(const nlohmann::json& data)
{
    if (data.is_array() && data.size() == 2)
    {
        std::string name = data[0];

        const nlohmann::json& objectProperties = data[1];

        if (objectProperties.contains("name") && objectProperties.contains("transform"))
        {
            // Deserialize the transform
            std::shared_ptr<Transform3D> transform = DeserializeTransform(objectProperties["transform"]);

            std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(name);
            newObject->SetTransform(transform);
            if (objectProperties.contains("cameraComponent"))
            {
                DeserializeCameraComponent(newObject, objectProperties["cameraComponent"]);
            }
            if (objectProperties.contains("meshComponent"))
            {
                //DeserializeMeshComponent(newObject, objectProperties["meshComponent"]);
            }

            return newObject;
        }

        Debug("Invalid data for GameObject in scene file");
    }
    else
    {
        Debug("Invalid data format for GameObject in scene file");
    }

    return nullptr;
}


std::shared_ptr<Transform3D> SceneSerializer::DeserializeTransform(const nlohmann::json& data)
{
    std::shared_ptr<Transform3D> transform = std::make_shared<Transform3D>();

    transform->Position.X(static_cast<float>(data["position"]["x"]));
    transform->Position.Y(static_cast<float>(data["position"]["y"]));
    transform->Position.Z(static_cast<float>(data["position"]["z"]));
    transform->Rotation.X(static_cast<float>(data["rotation"]["x"]));
    transform->Rotation.Y(static_cast<float>(data["rotation"]["y"]));
    transform->Rotation.Z(static_cast<float>(data["rotation"]["z"]));
    transform->Scale.X(static_cast<float>(data["scale"]["x"]));
    transform->Scale.Y(static_cast<float>(data["scale"]["y"]));
    transform->Scale.Z(static_cast<float>(data["scale"]["z"]));


    return transform;
}

std::shared_ptr<CameraComponent> SceneSerializer::DeserializeCameraComponent(
    std::shared_ptr<GameObject> gameObject, const json& cameraComponentData)
{
    std::string cameraType = cameraComponentData["camera"]["type"];

    if (cameraType == "OrthographicCamera")
    {
        std::shared_ptr<OrthographicCamera> orthoCamera = DeserializeOrthographicCamera(cameraComponentData["camera"]);
        if (orthoCamera)
        {
            std::shared_ptr<CameraComponent> cameraComponent = std::make_shared<CameraComponent>(gameObject);
            cameraComponent->SetCamera(orthoCamera);
            return cameraComponent;
        }
    }
    else if (cameraType == "PerspectiveCamera")
    {
        std::shared_ptr<PerspectiveCamera> perspCamera = DeserializePerspectiveCamera(cameraComponentData["camera"]);
        if (perspCamera)
        {
            std::shared_ptr<CameraComponent> cameraComponent = std::make_shared<CameraComponent>(gameObject);
            cameraComponent->SetCamera(perspCamera);
            return cameraComponent;
        }
    }

    return nullptr;
}

std::shared_ptr<OrthographicCamera> SceneSerializer::DeserializeOrthographicCamera(const nlohmann::json& data)
{
    float width = data["width"];
    float height = data["height"];
    float nearZ = data["near"];
    float farZ = data["far"];

    std::shared_ptr<OrthographicCamera> orthoCamera =
        std::make_shared<OrthographicCamera>(width, height, nearZ, farZ);

    return orthoCamera;
}

std::shared_ptr<PerspectiveCamera> SceneSerializer::DeserializePerspectiveCamera(const nlohmann::json& data)
{
    float width = data["width"];
    float height = data["height"];
    float nearZ = data["near"];
    float farZ = data["far"];
    float fov = data["fov"];

    std::shared_ptr<PerspectiveCamera> perspCamera =
        std::make_shared<PerspectiveCamera>(width, height, nearZ, farZ, fov);

    return perspCamera;
}