#pragma once

#include "std.h"

#include "Backend/ImGuiContext.h"
#include "Backend/VulkanHeader.h"

#include "Resource/Mesh.h"

#include "Renderer/RenderGraph/RenderPassEnum.h"
#include "Renderer/ShaderMap.h"

namespace vk
{
    class ShaderModule;
};

namespace wind
{
    // manage runtime resource and system
    class RasterShader;
    class ComputeShader;
    class GPUDevice;
    class MaterialManager;
    class Scene;
    class Engine;
    class Renderer;

    struct PathManager
    {
        std::filesystem::path projectPath;
        std::filesystem::path shaderPath;
    };

    struct RuntimeContext
    {
        void Init();
        void PostInit(const Window& window);
        void Quit();

        Scope<GPUDevice>    device;
        Scope<ImGUIContext> guiContext;
        Scope<Renderer>     renderer;
        Scene*              activeScene;
        PathManager         pathManager;
    };

    std::filesystem::path GetPath(std::filesystem::path path);

    extern RuntimeContext g_runtimeContext; // last entry to access the global info, try not use this

    // utils func
    class RuntimeUtils
    {
    public:
        static Renderer*  GetRenderer();
        static void       GPUWaitIdle();
        static vk::Device GetVulkanDevice();
        static Scene*     GetActiveScene();
    };

} // namespace wind
