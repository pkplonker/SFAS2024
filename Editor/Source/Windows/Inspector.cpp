#include "Inspector.h"

#include "Hierarchy.h"
#include "imgui.h"
#include "Engine/Implementation/GameObject.h"

Inspector::Inspector(std::shared_ptr<Hierarchy> hierarchy) : hierarchy(hierarchy), factory(std::make_unique<ComponentDrawerFactory>())
{
}

void Inspector::Draw()
{
	ImGui::Begin(INSPECTOR.c_str());

	if (const auto gameobject = hierarchy->GetSelectedObject().lock())
	{
		ImGui::Text(gameobject->Name.c_str());
		for (auto component : gameobject->GetComponents())
		{
			auto drawer = factory->CreateDrawer(component);
			if (drawer != nullptr)
			{
				drawer->Draw();
			}
		}
	}
	else
	{
		ImGui::Text("Select an object to inspect.");
	}

	ImGui::End();
}