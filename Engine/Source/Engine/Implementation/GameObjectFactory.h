#pragma once
#include <memory>
#include <string>

#include "GameObject.h"
#include "Engine/Math/Vector3.h"
#include "Engine/IComponent.h"

class IGraphics;
class IRenderable;
class IMaterial;
class Scene;
class CameraComponent;
class GameObject;

class GameObjectFactory
{
public:
    GameObjectFactory(std::shared_ptr<Scene> scene);

    GameObjectFactory(std::shared_ptr<Scene> scene, std::string name);

    void SetupRandom();

    template <typename T>
    GameObjectFactory& AddComponent()
    {
        static_assert(std::is_base_of<IComponent, T>::value, "T must be a derived class of IComponent");
        auto component = std::make_shared<T>(gameObject);
        if (component != nullptr)
        {
            gameObject->AddComponent(std::move(component));
            return *this;
        }
        return *this;
    }

    GameObjectFactory& AddPosition(Vec3 vec);

    GameObjectFactory& AddRotation(Vec3 vec);

    GameObjectFactory& AddScale(Vec3 vec);

    GameObjectFactory& AddName(std::string name);

    std::shared_ptr<GameObject> Build();

    // TODO: change to variadic arguments if possible?

#pragma region Component specific
    GameObjectFactory& AddSpriteRenderable(std::wstring shaderPath, std::wstring texturePath
    );
    GameObjectFactory& AddSpriteRenderable(std::wstring shaderPath);

    GameObjectFactory& AddMeshRenderable(std::string meshPath, std::wstring shaderPath, std::wstring texturePath);
    GameObjectFactory& AddMeshRenderable(std::string meshPath, std::wstring shaderPath);

    GameObjectFactory& AddRandomRotation();

    GameObjectFactory& AddPerspectiveCamera();

    GameObjectFactory& AddOrthoCamera();
    static void Init(IGraphics* graphics);

#pragma endregion

private:
    std::shared_ptr<GameObject> gameObject;
    std::shared_ptr<Scene> scene;
    inline static bool random = false;
    inline static IGraphics* graphics = nullptr;
};
