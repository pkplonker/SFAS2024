#pragma once
#include <string>

class EditorWindow
{
public:
	EditorWindow() = default;
	virtual ~EditorWindow() = default;

	virtual void Draw() = 0;
	virtual std::string GetName() = 0;
};
