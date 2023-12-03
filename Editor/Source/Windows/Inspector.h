#pragma once
#include <memory>

#include "ComponentDrawerFactory.h"
#include "ImGuiWindow.h"
#include "RenamingHelper.h"
#include "Engine/Math/Vector3.h"
class Hierarchy;
const std::string INSPECTOR = "Inspector";

class Inspector : public EditorWindow

{
public:

	Inspector(std::shared_ptr<Hierarchy> hierarchy);

	void Draw() override;
	static void DrawVector(const char* vectorName, Vec3& vector);
	std::string GetName() override { return INSPECTOR; }
private:
	std::shared_ptr<Hierarchy> hierarchy;
	std::unique_ptr<ComponentDrawerFactory> factory;
	RenamingHelper renamingHelper;

};
