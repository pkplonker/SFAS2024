#pragma once
#include <d3d11.h>
#include <string>

class IShader
{
public:
    virtual ~IShader() = default;
    virtual void Reload() =0;
   

    IShader(const std::wstring& path) : path(path)
    {
    }

    virtual void Update() = 0;
    virtual std::wstring& GetPath() { return path; }

protected:
    std::wstring path;
};

