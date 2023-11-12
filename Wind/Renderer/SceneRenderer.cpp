#include "SceneRenderer.h"

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Descriptor.h"
#include "RenderGraph/RenderPass.h"
#include "Renderer/MeshPass.h"
#include "Renderer/RenderGraph/RenderPassEnum.h"
#include "Resource/Loader.h"
#include "Resource/Mesh.h"
#include "Resource/VertexFactory.h"
#include "Scene/Scene.h"
#include "View.h"

namespace wind {

void SceneRenderer::Init() {}

void SceneRenderer::InitView(View& view) {
    for (auto meshPassType = MeshPassType::BasePass; meshPassType != MeshPassType::Count;
         meshPassType      = Step(meshPassType)) {
        BuildMeshDrawCommand(m_renderScene->m_meshPasses[meshPassType]);
    }
}

void SceneRenderer::SetViewPort(u32 width, u32 height) {
    m_viewPortWidth  = width;
    m_viewPortHeight = height;
}

void SceneRenderer::Render(View& view, RenderGraph& renderGraph) {
    m_renderScene = RuntimeUtils::GetActiveScene();

    InitView(view);

    m_Presentpass = renderGraph.AddPass("PresentPass", RenderCommandQueueType::Graphics);
    m_Presentpass->MarkWriteBackBuffer();

    m_Presentpass->SetRenderExecCallBack([&](RenderEncoder& encoder) {
        for (auto& meshDrawCommand : m_cacheMeshDrawCommands[MeshPassType::BasePass]) {
            auto pso = g_runtimeContext.psoCache->GetPso(meshDrawCommand.pipelineID);

            encoder.SetViewport(m_viewPortWidth, m_viewPortHeight, 0.0, 1.0);
            encoder.SetScissor(0, 0, m_viewPortWidth, m_viewPortHeight);

            auto vertexBuffer = meshDrawCommand.drawMesh.meshSource->vertexBuffer;
            auto indexBuffer  = meshDrawCommand.drawMesh.meshSource->indexBuffer;

            encoder.BindPSO(pso);
            encoder.BindVertexBuffer(0, 1, vertexBuffer->GetNativeHandle(), 0);
            encoder.BindIndexBuffer(indexBuffer->GetNativeHandle(), 0, vk::IndexType::eUint32);

            encoder.DrawIndexed(3 * meshDrawCommand.drawMesh.meshSource->indices.size(), 1, 0, 0,
                                0);
        }
    });

    renderGraph.Exec();
}

void SceneRenderer::BuildMeshDrawCommand(const MeshPass& meshPass) {
    auto                psocache      = g_runtimeContext.psoCache.get();
    RenderGraphPassType graphPassType = meshPass.type == MeshPassType::BasePass
                                            ? RenderGraphPassType::MeshPassMRT
                                            : RenderGraphPassType::MeshPass;
    m_cacheMeshDrawCommands[meshPass.type].clear();
    for (auto meshProxy : meshPass.staticMeshes) {
        MeshDrawCommand meshDrawCommand;
        meshDrawCommand.drawMesh.firstVertex = 0;
        meshDrawCommand.drawMesh.meshSource  = &meshProxy->meshSource;
        meshDrawCommand.drawMesh.vertexCount = meshProxy->meshSource.vertices.size();
        meshDrawCommand.drawMesh.indexCount  = meshProxy->meshSource.indices.size();
        meshDrawCommand.materialProxy        = meshProxy->material;

        meshDrawCommand.pipelineID =
            psocache->CachePso(*meshProxy->material, VertexFactoryType::StaicMesh, graphPassType);

        m_cacheMeshDrawCommands[meshPass.type].push_back(meshDrawCommand);
    }
}

} // namespace wind