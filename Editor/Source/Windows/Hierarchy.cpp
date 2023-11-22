#include "Hierarchy.h"

#include <memory>
#include <vector>

#include "imgui.h"
#include "Engine/Implementation/GameObject.h"
#include "Engine/Implementation/Scene.h"

Hierarchy::Hierarchy(const std::weak_ptr<Scene>& scene) : scene(scene)
{
}

void Hierarchy::Draw()
{
	ImGui::Begin(HIERARCHY.c_str());
	std::vector<std::shared_ptr<GameObject>> objectsToRemove;

	if (auto sharedScene = scene.lock())
	{
		for (const auto& object : sharedScene->GetObjects())
		{
			std::string label = object->Name;
			bool is_selected = (selectedObject.lock() == object);

			if (ImGui::Selectable(label.c_str(), is_selected))
			{
				selectedObject = object;
			}

			if (ImGui::BeginPopupContextItem(("ObjectContextMenu" + label).c_str()))
			{
				selectedObject = object;
				if (ImGui::MenuItem("Delete"))
				{
					objectsToRemove.emplace_back(object);
				}

				ImGui::EndPopup();
			}
		}
		for (const auto object : objectsToRemove)
		{
			sharedScene->RemoveObject(object);
		}
	}
	ImGui::End();
}

std::weak_ptr<GameObject> Hierarchy::GetSelectedObject()
{
	return selectedObject;
}