#include "PerspectiveCamera.h"

#include "Engine/Implementation/Transform3D.h"

PerspectiveCamera::PerspectiveCamera(std::shared_ptr<Transform3D> transform, float width, float height, float near,
	float far, float FOV) : transform(transform), near(near),
	far(far), width(width), height(height), FOV(FOV),
	view(DirectX::XMMatrixIdentity()),
	projection(DirectX::XMMatrixIdentity()),
	vpMatrix(DirectX::XMMatrixIdentity())
{
}

DirectX::XMMATRIX PerspectiveCamera::GetProjectionMatrix()
{
	projection = DirectX::XMMatrixPerspectiveFovLH(FOV, width / height, near, far);
	return projection;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewMatrix()
{
	view = DirectX::XMMatrixLookAtLH(GetEyePosition(), GetFocusPoint(), GetUpDirection());
	return view;
}

DirectX::XMMATRIX PerspectiveCamera::GetViewProjectionMatrix()
{
	GetProjectionMatrix();
	GetViewMatrix();
	vpMatrix = XMMatrixMultiply(view, projection);
	return vpMatrix;
}

DirectX::XMVECTOR PerspectiveCamera::GetFocusPoint()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		transform->Rotation.X(), transform->Rotation.Y(), transform->Rotation.Z());
	DirectX::XMVECTOR forwardBase = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR forwardDirection = DirectX::XMVector3TransformNormal(forwardBase, rotationMatrix);
	return DirectX::XMVectorAdd(GetEyePosition(), forwardDirection);
}

DirectX::XMVECTOR PerspectiveCamera::GetEyePosition()
{
	return DirectX::XMVectorSet(transform->Position.X(), transform->Position.Y(), transform->Position.Z(), 1.0f);;
}

DirectX::XMVECTOR PerspectiveCamera::GetUpDirection()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(
		transform->Rotation.X(), transform->Rotation.Y(), transform->Rotation.Z());

	DirectX::XMVECTOR upBase = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMVector3TransformNormal(upBase, rotationMatrix);
}

void PerspectiveCamera::ImGuiDraw()
{
	/*if (ImGui::TreeNodeEx("Perspective Camera Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginPopupContextItem("PerspectiveCameraContext"))
		{
			if (ImGui::MenuItem("Reset Near"))
			{
				near = DEFAULT_NEAR;
			}
			if (ImGui::MenuItem("Reset Far"))
			{
				far = DEFAULT_FAR;
			}
			if (ImGui::MenuItem("Reset FOV"))
			{
				FOV = DEFAULT_FOV;
			}
			ImGui::EndPopup();
		}
		ImGui::SliderFloat("Field of View", &FOV, 0.01f, 3.141f);
		ImGui::DragFloat("Width", &width, 1, 1);
		ImGui::DragFloat("Height", &height, 1, 1);
		ImGui::DragFloat("Near Plane", &near, 1, 0.1f);
		ImGui::DragFloat("Far Plane", &far, 1, 0.1f);
		ImGui::TreePop();*/
		//}
}

void PerspectiveCamera::SetHeight(int height)
{
	this->height = static_cast<float>(height);
}

void PerspectiveCamera::SetWidth(int width)
{
	this->width = static_cast<float>(width);
}

void PerspectiveCamera::SetNear(float near)
{
	this->near = near;
}

void PerspectiveCamera::SetFar(float far)
{
	this->far = far;
}

void PerspectiveCamera::SetFOV(float fov)
{
	this->FOV = fov;
}

std::shared_ptr<Transform3D> PerspectiveCamera::GetTransform()
{
	return transform;
}