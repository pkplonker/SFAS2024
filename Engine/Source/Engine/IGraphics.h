#pragma once

#include <map>
#include <list>
#include <memory>

class ICamera;
class IRenderable;
class IShader;
class ITexture;

class IGraphics
{
public:
	IGraphics();
	virtual ~IGraphics();

	virtual void Update() = 0;
	virtual bool IsValid() = 0;
	virtual void PostUpdate() = 0;
	virtual ITexture* CreateTexture(const wchar_t* filepath) = 0;
	virtual IShader* CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader,
		const char* psentry, const char* psshader, ITexture* TextureIn) = 0;
	virtual std::shared_ptr<IRenderable> CreateBillboard(IShader* ShaderIn) = 0;
	virtual std::shared_ptr<IRenderable> CreateMeshRenderable(IShader* ShaderIn) = 0;
	virtual void SetActiveCamera(std::shared_ptr<ICamera> camera) = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void RemoveRenderable(const std::shared_ptr<IRenderable>& shared) = 0;
	virtual void SetRenderToTexture(bool state) = 0;
protected:
	std::map<IShader*, std::list<std::shared_ptr<IRenderable>>> Renderables;
	std::list<ITexture*> Textures;
};
