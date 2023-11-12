#pragma once
#include <memory>

#include "GameObject.h"
#include "IComponent.h"
#include "IRenderable.h"
#include "Implementation/MeshRenderable.h"
#include "Implementation/Scene.h"
#include "Implementation/SpriteRenderable.h"

class GameObject;

class GameObjectFactory
{
public:
    GameObjectFactory(std::shared_ptr<Scene> scene) : scene(scene)
    {
        gameObject = std::make_shared<GameObject>();
    }

    GameObjectFactory(std::shared_ptr<Scene> scene, std::string name) : scene(scene)
    {
        gameObject = std::make_shared<GameObject>(name);
    }

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

    GameObjectFactory& AddPosition(Vec3 vec)
    {
        gameObject->Transform()->Position = vec;
        return *this;
    }

    GameObjectFactory& AddRotation(Vec3 vec)
    {
        gameObject->Transform()->Rotation = vec;
        return *this;
    }

    GameObjectFactory& AddScale(Vec3 vec)
    {
        gameObject->Transform()->Scale = vec;
        return *this;
    }

    GameObjectFactory& AddName(std::string name)
    {
        gameObject->Name = name;
        return *this;
    }

    std::shared_ptr<GameObject> Build()
    {
        scene->AddObject(gameObject);
        return gameObject;
    }

    // TODO: change to variadic arguments if possible?

#pragma region Component specific
    GameObjectFactory& AddSpriteRenderable(std::shared_ptr<IRenderable> renderable)
    {
        auto component = std::make_shared<SpriteRenderable>(gameObject, renderable);
        if (component != nullptr)
        {
            gameObject->AddComponent(std::move(component));
            return *this;
        }
        return *this;
    }

    GameObjectFactory& AddMeshRenderable(std::shared_ptr<IRenderable> renderable)
    {
        auto component = std::make_shared<MeshRenderable>(gameObject, renderable);
        if (component != nullptr)
        {
            gameObject->AddComponent(std::move(component));
            return *this;
        }
        return *this;
    }

    GameObjectFactory& AddRandomRotation()
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        gameObject->Transform()->Rotation = Vec3(static_cast<float>(std::rand() % 361), static_cast<float>(std::rand() % 361), static_cast<float>(std::rand() % 361));
        return *this;
    }


#pragma endregion

private:
    std::shared_ptr<GameObject> gameObject;
    std::shared_ptr<Scene> scene;
};
