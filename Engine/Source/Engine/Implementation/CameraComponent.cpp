#include "CameraComponent.h"

#include "imgui.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/Transform3D.h"

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object) : IComponent(object), camera(nullptr)
{
}

std::shared_ptr<ICamera> CameraComponent::GetCamera()
{
	return camera;
}

DirectX::XMMATRIX CameraComponent::GetViewMatrix()
{
	return camera->GetViewMatrix();
}

CameraComponent::CameraComponent(std::weak_ptr<GameObject> object, std::shared_ptr<ICamera> camera) : IComponent(object),
camera(camera)
{
}

void CameraComponent::SetCamera(std::shared_ptr<ICamera> cam)
{
	camera = cam;
}

DirectX::XMMATRIX CameraComponent::GetProjectionMatrix()
{
	return camera->GetProjectionMatrix();
}

DirectX::XMMATRIX CameraComponent::GetViewProjectionMatrix()
{
	return camera->GetViewProjectionMatrix();
}

void CameraComponent::SetHeight(int height)
{
	camera->SetHeight(height);
}

void CameraComponent::SetWidth(int width)
{
	camera->SetWidth(width);;
}

void CameraComponent::ImGuiDraw()
{
	if (ImGui::CollapsingHeader("Camera Component", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginPopupContextItem("CameraComponentContext"))
		{
			if (ImGui::MenuItem("Delete component"))
			{
				if (auto obj = gameObject.lock())
				{
					obj->RemoveComponent(shared_from_this());
				}
			}
			ImGui::EndPopup();
		}

		if (camera)
		{
			if (auto cam = std::dynamic_pointer_cast<IimGuiDraw>(camera))
			{
				ImGui::Columns(2, nullptr, false);
				ImGui::Text("Camera Settings");
				cam->ImGuiDraw();
				ImGui::NextColumn();
				ImGui::Text("Transform Settings");
				camera->GetTransform()->ImGuiDraw();
				ImGui::Columns(1);
			}
		}
		else
		{
			ImGui::Text("No camera attached");
		}
	}
}

std::shared_ptr<Transform3D> CameraComponent::GetTransform()
{
	if (auto object = gameObject.lock())
	{
		return object->Transform();
	}
	return nullptr;
}