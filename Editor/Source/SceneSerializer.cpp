#include "SceneSerializer.h"

#include <fstream>
#include <string>
#include "json.hpp"
#include "Engine/Implementation/Debug.h"
#include "Engine/Implementation/Scene.h"
using json = nlohmann::json;

SceneSerializer::SceneSerializer(std::weak_ptr<Scene> scene)
{
    SceneSerializer::scene = scene;
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

bool SceneSerializer::Serialize()
{
    std::string path = "S:/Users/pkplo/OneDrive/Documents/C++/SFAS2024/Editor/Resource/Scenes/TestScene.scene";
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
