#pragma once
#include "ComponentDrawer.h"
#include "OrthographicCamera.h"
#include "Engine/Implementation/PerspectiveCamera.h"

class CameraComponent;

class CameraComponentDrawer : public ComponentDrawer
{
public:
	virtual ~CameraComponentDrawer();
	CameraComponentDrawer(std::weak_ptr<CameraComponent> component);
	static void DrawPerspective(std::shared_ptr<PerspectiveCamera> cam);
	static void DrawOrtho(std::shared_ptr<OrthographicCamera> cam);
	static void DrawCameraControls(std::shared_ptr<ICamera> camera);
	void Draw() override;
private:
	std::weak_ptr<CameraComponent> component;
};
