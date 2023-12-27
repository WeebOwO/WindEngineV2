#include "RuntimeContext.h"

#include "std.h"
#include <shaderc/shaderc.hpp>

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "Resource/VertexFactory.h"

#include "Backend/ComputeShader.h"
#include "Backend/Device.h"
#include "Backend/PipelineBuilder.h"
#include "Backend/RasterShader.h"
#include "Backend/Shader.h"

#include "Renderer/Material.h"
#include "Renderer/Renderer.h"

namespace wind
{
    RuntimeContext g_runtimeContext;

    

    void RuntimeContext::Init()
    {
        // Core engine part
        Log::Init();
        device = scope::Create<GPUDevice>();

        // init the renderer
        renderer = scope::Create<Renderer>(*device);
        renderer->Init();

        // init project path
        auto currentPath = std::filesystem::current_path();

        pathManager.projectPath = currentPath.parent_path().parent_path().parent_path().parent_path();
        pathManager.shaderPath  = pathManager.projectPath.append("Shaders");
    }

    void RuntimeContext::Quit()
    {
        device->WaitIdle();
        renderer->Quit();
        guiContext->Quit(*device);
    }

    void RuntimeContext::PostInit(const Window& window)
    {
        guiContext = scope::Create<ImGUIContext>();
        guiContext->Init(*device, window);
    }

    std::filesystem::path GetPath(std::filesystem::path path) { return std::filesystem::path(); }

    void RuntimeUtils::GPUWaitIdle()
    {
        assert(g_runtimeContext.device != nullptr);
        g_runtimeContext.device->GetVkDeviceHandle().waitIdle();
    }

    vk::Device RuntimeUtils::GetVulkanDevice()
    {
        assert(g_runtimeContext.device != nullptr);
        return g_runtimeContext.device->GetVkDeviceHandle();
    }

    Scene* RuntimeUtils::GetActiveScene()
    {
        assert(g_runtimeContext.activeScene != nullptr);
        return g_runtimeContext.activeScene;
    }

    Renderer*  RuntimeUtils::GetRenderer() {
        assert(g_runtimeContext.renderer != nullptr);
        return g_runtimeContext.renderer.get();
    }
} // namespace wind