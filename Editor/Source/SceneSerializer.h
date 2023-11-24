#pragma once
#include <json.hpp>
#include <memory>

struct Transform3D;
class GameObject;
class Scene;

class SceneSerializer
{
public:
    SceneSerializer(std::weak_ptr<Scene> scene);
    static void WriteToFile(nlohmann::json sceneData, std::string path);
    static nlohmann::json SerializeGameObject(const std::shared_ptr<GameObject>& object);
    static nlohmann::json SerializeTransform(std::shared_ptr<Transform3D> transform);

    static bool Serialize();

private:
   inline static std::weak_ptr<Scene> scene;
};
