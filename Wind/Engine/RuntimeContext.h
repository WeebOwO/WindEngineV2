#pragma once

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h" 
}

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
    class GPUDevice;
    class Scene;
    class Renderer;

    struct PathManager
    {
        std::filesystem::path projectPath;
        std::filesystem::path shaderPath;
        std::filesystem::path configPath;
        std::filesystem::path asssetPath;
    };

    struct RuntimeContext
    {
        void Init();
        void PostInit(const Window& window);
        void Quit();

        Scope<GPUDevice>    device      = nullptr;
        Scope<ImGUIContext> guiContext  = nullptr;
        Scope<Renderer>     renderer    = nullptr;
        lua_State*          luaState    = nullptr;
        Scene*              activeScene = nullptr;
        PathManager         pathManager;
    };

    std::filesystem::path GetPath(std::filesystem::path path);

    extern RuntimeContext g_runtimeContext; // last entry to access the global info, try not use this
} // namespace wind
