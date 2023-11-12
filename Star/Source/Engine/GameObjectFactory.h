#pragma once
#include <memory>

#include "GameObject.h"
#include "IComponent.h"
#include "IRenderable.h"
#include "Implementation/SpriteRenderable.h"

class GameObject;

class GameObjectFactory
{
public:
    GameObjectFactory()
    {
        gameObject = std::make_shared<GameObject>();
    }

    GameObjectFactory(std::string name)
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


#pragma endregion

private:
    std::shared_ptr<GameObject> gameObject;
};
