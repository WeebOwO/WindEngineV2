#pragma once

#include "std.h"

#include "MeshPass.h"
#include "RenderGraph/RenderGraph.h"

#include "Engine/RenderThread.h"

#include "Resource/Texture.h"

#include "Backend/Fwd.h"
#include "Backend/Command.h"

namespace wind {
class Scene;
class View;

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
    void DrawMesh(CommandEncoder& encoder);

    Scene*      m_renderScene;
    FrameParms* m_framedata;

    vk::Viewport m_viewPort{};

    PerpassData<std::vector<MeshDrawCommand>> m_cacheMeshDrawCommands;

    uint32_t m_viewPortWidth  = 0;
    uint32_t m_viewPortHeight = 0;
};
} // namespace wind