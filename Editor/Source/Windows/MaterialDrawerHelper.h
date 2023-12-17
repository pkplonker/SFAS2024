#pragma once
#include <memory>

class IRenderableComponent;

class MaterialDrawerHelper
{
public:
    MaterialDrawerHelper(std::weak_ptr<IRenderableComponent> component);
    void DrawMaterial();
    void ChangeShader();
    void ChangeTexture();
    void OpenShader() const;
private:
    std::weak_ptr<IRenderableComponent> component;
};
