#pragma once
#include <memory>

class GameObject;

class IComponent
{
public:
    IComponent(std::weak_ptr<GameObject> object);
    virtual ~IComponent() = default;
    std::weak_ptr<GameObject> GetGameObject();
    virtual void Remove();
    virtual void SetIsEnabled(bool state);
    virtual bool GetIsEnabled();
    std::weak_ptr<GameObject> gameObject;

private:
    bool isEnabled = false;
};
