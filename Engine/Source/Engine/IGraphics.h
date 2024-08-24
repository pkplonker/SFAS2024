#pragma once

#include <map>
#include <list>
#include <memory>
#include <vector>

#include "IRenderStrategy.h"
#include "IResizeHandler.h"
#include "RenderingStats.h"
#include "Implementation/DirectX11/DirectX11Shader.h"

class IMeshRenderable;
class MeshComponent;
class Mesh;
class ICamera;
class IRenderable;
class IShader;
class ITexture;
class IMaterial;

class IGraphics : public IResizeHandler
{
public:
    IGraphics();
    virtual ~IGraphics();

    virtual void Update() = 0;
    virtual bool IsValid() = 0;
    virtual void PostUpdate() = 0;
    virtual ITexture* CreateTexture(const wchar_t* filepath) = 0;
    virtual IShader* CreateShader(const wchar_t* filepath, const char* vsentry, const char* vsshader,
                                  const char* psentry, const char* psshader) = 0;
    virtual std::shared_ptr<IRenderable> CreateBillboard(IMaterial* material) = 0;
    virtual std::shared_ptr<IMeshRenderable> CreateMeshRenderable(IMaterial* material, Mesh* mesh) =0;
    virtual std::shared_ptr<IMeshRenderable> CreateMeshRenderable(Mesh* mesh) =0;

    virtual void SetActiveCamera(std::shared_ptr<ICamera> camera) = 0;
    virtual int GetWidth() = 0;
    virtual int GetHeight() = 0;
    virtual void RemoveRenderable(const std::shared_ptr<IRenderable>& shared) = 0;
    virtual void SetRenderToTexture(int width, int height) = 0;
    virtual IMaterial* CreateMaterial(IShader* shader, ITexture* texture) =0;
    virtual void UpdateRenderable(IMaterial* mat, const std::shared_ptr<IRenderable>& shared) =0;
    virtual void WindowResize(int width, int height) = 0;
    virtual bool TryUpdateShader(IShader* shader, const char* vsentry, const char* vsshader,
                                 const char* psentry, const char* psshader) = 0;
    RenderingStats currentStats;
    void AddRenderStrategy(std::shared_ptr<IRenderStrategy> strategy);
    void RemoveRenderStrategy(std::shared_ptr<IRenderStrategy> strategy);
protected:
    std::map<IMaterial*, std::list<std::shared_ptr<IRenderable>>> Renderables;
    std::list<ITexture*> Textures;
    std::vector<std::shared_ptr<IRenderStrategy>> postRenderStrategies;

};
