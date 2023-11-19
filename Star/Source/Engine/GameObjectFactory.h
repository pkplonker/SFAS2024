#pragma once
#include <memory>

#include "GameObject.h"
#include "IComponent.h"
#include "IRenderable.h"
#include "OrthographicCamera.h"
#include "Implementation/CameraComponent.h"
#include "Implementation/MeshRenderable.h"
#include "PerspectiveCamera.h"
#include "Implementation/Scene.h"
#include "Implementation/SpriteRenderable.h"

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

    GameObjectFactory& AddScale(Vec3 vec)
    {
        gameObject->Transform()->Scale = vec;
        return *this;
    }

    GameObjectFactory& AddName(std::string name);

    std::shared_ptr<GameObject> Build();

    // TODO: change to variadic arguments if possible?

#pragma region Component specific
    GameObjectFactory& AddSpriteRenderable(std::shared_ptr<IRenderable> renderable);

    GameObjectFactory& AddMeshRenderable(std::shared_ptr<IRenderable> renderable);

    GameObjectFactory& AddRandomRotation();

    GameObjectFactory& AddPerspectiveCamera();

    GameObjectFactory& AddOrthoCamera();


#pragma endregion

private:
    std::shared_ptr<GameObject> gameObject;
    std::shared_ptr<Scene> scene;
    inline static bool random = false;
};
