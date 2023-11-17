#pragma once

#include "std.h"

#include "RenderBackend/ImGuiContext.h"
#include "RenderBackend/VulkanHeader.h"
#include "Renderer/RenderGraph/RenderPassEnum.h"
#include "Resource/Mesh.h"

namespace vk {
class ShaderModule;
};

namespace wind {
// manage runtime resource and system
class RasterShader;
class ComputeShader;
class GPUDevice;
class MaterialManager;
class Scene;
class Engine;

class ShaderMap {
public:
    ShaderMap();

    void CacheRasterShader(Ref<RasterShader> shader);
    void CacheComputeShader(Ref<ComputeShader> shader);

    Ref<RasterShader> GetRasterShader(const std::string& shaderName) noexcept {
        assert(m_rasterShaderCache.contains(shaderName));
        return m_rasterShaderCache[shaderName];
    }

    Ref<ComputeShader> GetComputeShader(const std::string& shaderName) noexcept {
        assert(m_computeShaderCache.contains(shaderName));
        return m_computeShaderCache[shaderName];
    }

private:
    vk::Device                                          m_device;
    std::unordered_map<std::string, Ref<RasterShader>>  m_rasterShaderCache;
    std::unordered_map<std::string, Ref<ComputeShader>> m_computeShaderCache;
};

struct PathManager {
    std::filesystem::path projectPath;
    std::filesystem::path shaderPath;
};

class PsoCache {
public:
    PsoCache();

    u64          CachePso(const Material& material, VertexFactoryType vertextype,
                          RenderGraphPassType graphPassType);
    vk::Pipeline GetPso(u64 pipelineStateID);

    void Destroy();

private:
    friend class RuntimeContext;

    vk::Device                            m_device;
    std::unordered_map<u64, vk::Pipeline> m_pipelineCache;
};

struct RuntimeContext {
    void Init();
    void PostInit(const Window& window);
    void Quit();

    Scope<ShaderMap>       shaderMap;
    Scope<GPUDevice>       device;
    Scope<MaterialManager> materialManager;
    Scope<PsoCache>        psoCache;
    Scope<ImGUIContext>    guiContext;
    Scene*                 activeScene;

    PathManager pathManager;
};

std::filesystem::path GetPath(std::filesystem::path path);

extern RuntimeContext g_runtimeContext;

// utils func
class RuntimeUtils {
public:
    static void       GPUWaitIdle();
    static vk::Device GetVulkanDevice();
    static Scene*     GetActiveScene();
};

} // namespace wind
