#pragma once
#include <DirectXMath.h>
#include <memory>
#include <set>

class GameObject;

struct Transform : std::enable_shared_from_this<Transform>
{
    virtual ~Transform() = default;

    Transform()
    {
    }

    void SetGameObject(std::weak_ptr<GameObject> go) { this->gameobject = go; }

    void SetParent(const std::shared_ptr<Transform>& newParent)
    {
        if (const auto oldParent = parent.lock())
        {
            oldParent->children.erase(shared_from_this());
        }

        parent = newParent;

        if (newParent)
        {
            newParent->children.insert(shared_from_this());
        }
    }

    void SetParentWeak(const std::weak_ptr<Transform>& newParent)
    {
        if (auto newParentShared = newParent.lock())
        {
            SetParent(newParentShared);
        }
    }

    bool HasChildren() const
    {
        return children.size() > 0;
    }

    struct TransformCompare
    {
        bool operator()(const std::weak_ptr<Transform>& lhs, const std::weak_ptr<Transform>& rhs) const
        {
            return lhs.lock().get() < rhs.lock().get();
        }
    };

    std::weak_ptr<Transform> parent;
    std::set<std::weak_ptr<Transform>, TransformCompare> children;
    std::weak_ptr<GameObject> gameobject;

    virtual DirectX::XMMATRIX GetWorldMatrix()=0;

private:
    void AddChild(const std::shared_ptr<Transform>& child)
    {
        if (child && child->parent.lock() != shared_from_this())
        {
            child->SetParent(shared_from_this());
        }
    }

    void RemoveChild(const std::shared_ptr<Transform>& child)
    {
        if (child)
        {
            children.erase(child);
        }
    }
};
