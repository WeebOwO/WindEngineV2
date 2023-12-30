#include "RuntimeContext.h"

#include "lua.h"
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

        pathManager.projectPath = WORK_DIR;
        pathManager.shaderPath  = pathManager.projectPath.append("Shaders");
        pathManager.asssetPath  = pathManager.projectPath.append("Assets");

        // init the lua virutal machine
        luaState = luaL_newstate();
        if(luaState == NULL) {
            WIND_CORE_ERROR("Fail to create lua machine");
        } 
    }

    void RuntimeContext::Quit()
    {
        device->WaitIdle();
        renderer->Quit();
        guiContext->Quit(*device);
        lua_close(luaState);
    }

    void RuntimeContext::PostInit(const Window& window)
    {
        guiContext = scope::Create<ImGUIContext>();
        guiContext->Init(*device, window);
    }

    std::filesystem::path GetPath(std::filesystem::path path) { return std::filesystem::path(); }
} // namespace wind