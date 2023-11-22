#include "CameraComponentDrawer.h"

#include <utility>

#include "imgui.h"
#include "Engine/Implementation/CameraComponent.h"

CameraComponentDrawer::~CameraComponentDrawer() = default;

CameraComponentDrawer::CameraComponentDrawer(std::weak_ptr<IComponent> component) : ComponentDrawer(std::move(component))
{
}

void CameraComponentDrawer::Draw()
{
	if (auto cameraComponent = component.lock())
	{
		ImGui::Text("Camera Test.");
	}
}