#pragma once
#include "Implementation/DirectX11/DirectX11Graphics.h"
#include "Engine/IUpdatePipe.h"

class ImGuiController : public IUpdatePipe
{
public:
    ImGuiController()=default;
    void Init(DirectX11Graphics* Graphics, IInput* Input);
    void PreUpdate() override ;

    void Update() override ;
    void PostUpdate() override ;

    void ShutDown() override;
};
