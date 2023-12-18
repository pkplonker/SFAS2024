#pragma once
#include <memory>
#include <set>

class GameObject;

struct Transform
{
    Transform()
    {
    }

    void SetGameObject(std::weak_ptr<GameObject> go) { this->gameobject = go; }

    std::weak_ptr<Transform> parent;
    std::set<std::weak_ptr<Transform>> children;
    std::weak_ptr<GameObject> gameobject;
};
