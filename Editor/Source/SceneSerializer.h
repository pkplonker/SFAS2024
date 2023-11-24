#pragma once
#include <json.hpp>
#include <memory>

class CameraComponent;
class OrthographicCamera;
class PerspectiveCamera;
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
    static nlohmann::json SerializeCameraComponent(std::shared_ptr<CameraComponent> camera);
    static nlohmann::json SerializeOrthographicCamera(std::shared_ptr<OrthographicCamera> camera);
    static nlohmann::json SerializePerspectiveCamera(std::shared_ptr<PerspectiveCamera> camera);

    static std::shared_ptr<Scene> Deserialize();

    static std::shared_ptr<GameObject> DeserializeGameObject(const nlohmann::json& data);
    static std::shared_ptr<Transform3D> DeserializeTransform(const nlohmann::json& data);
    static std::shared_ptr<CameraComponent> DeserializeCameraComponent(std::shared_ptr<GameObject> gameObject,
                                                                       const nlohmann::json& data);
    static std::shared_ptr<OrthographicCamera> DeserializeOrthographicCamera(const nlohmann::json& data);
    static std::shared_ptr<PerspectiveCamera> DeserializePerspectiveCamera(const nlohmann::json& data);
    static bool Serialize();

private:
    inline static std::weak_ptr<Scene> scene;
};
