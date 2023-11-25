#pragma once
#include <string>
class IShader
{
public:
	virtual ~IShader() = default;

	IShader(const std::wstring& path) : path(path){}
	virtual void Update() = 0;
	virtual std::wstring GetPath(){return path;}

protected:
	std::wstring path;
};

