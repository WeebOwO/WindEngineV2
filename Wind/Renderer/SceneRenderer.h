#pragma once

#include "Engine/RenderThread.h"
#include "Resource/Texture.h"
#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Fwd.h"
#include "RenderGraph/RenderGraph.h"
#include "Renderbackend/SwapChain.h"
#include "Renderer/MeshPass.h"

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

    vk::Viewport m_viewPort;
    // all the renderpass are create by rendergraph
    RenderGraphPass* m_Presentpass;

    std::array<std::vector<MeshDrawCommand>, MeshPassType::Count> m_cacheMeshDrawCommands;
};
} // namespace wind