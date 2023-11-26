#pragma once

#include "std.h"

#include "MeshPass.h"
#include "RenderGraph/RenderGraph.h"

#include "Engine/RenderThread.h"

#include "Resource/Texture.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Fwd.h"
#include "Renderbackend/SwapChain.h"


namespace wind {
class Scene;
class View;

struct RenderConfig {
    uint32_t MAX_FRAME_IN_FLIGHT = 2;
};

class SceneRenderer {
public:
    SceneRenderer()  = default;
    ~SceneRenderer() = default;

    void SetViewPort(float offsetX, float offsetY, float width, float height);

    void Render(View& view, RenderGraph& renderGraph);
    void Init();

private:
    void InitView(View& view); // Dispatch MeshPass

    void BuildMeshDrawCommand(const MeshPass& meshPass);

    Scene*      m_renderScene;
    FrameParms* m_framedata;

    vk::Viewport m_viewPort{};

    PerpassData<std::vector<MeshDrawCommand>> m_cacheMeshDrawCommands;

    u32 m_viewPortWidth  = 0;
    u32 m_viewPortHeight = 0;
};
} // namespace wind