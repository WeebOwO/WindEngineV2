#include "SceneRenderer.h"

#include "Core/Log.h"
#include "Engine/RuntimeContext.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Descriptor.h"
#include "RenderGraph/RenderPass.h"
#include "Renderer/MeshPass.h"
#include "Renderer/RenderGraph/RenderPass.h"
#include "Renderer/RenderGraph/RenderPassEnum.h"
#include "Resource/Loader.h"
#include "Resource/Mesh.h"
#include "Resource/VertexFactory.h"
#include "Scene/Scene.h"
#include "View.h"
#include "imgui.h"

namespace wind {

void SceneRenderer::Init() {}

void SceneRenderer::InitView(View& view) {
    for (auto meshPassType = MeshPassType::BasePass; meshPassType != MeshPassType::Count;
         meshPassType      = Step(meshPassType)) {
        BuildMeshDrawCommand(m_renderScene->m_meshPasses[meshPassType]);
    }
}

void SceneRenderer::SetViewPort(float offsetX, float offsetY, float width, float height) {
    m_viewPort.setWidth(width)
        .setHeight(height)
        .setX(offsetX)
        .setY(offsetY)
        .setMinDepth(0.0)
        .setMaxDepth(1.0);
}

void SceneRenderer::Render(View& view, RenderGraph& renderGraph) {
    m_renderScene = RuntimeUtils::GetActiveScene();

    InitView(view);

    auto lightPass = renderGraph.AddPass("LightPass", RenderCommandQueueType::Graphics);
    lightPass->MarkWriteBackBuffer();
    lightPass->SetRenderExecCallBack([&](RenderEncoder& encoder) {
        for (auto& meshDrawCommand : m_cacheMeshDrawCommands[MeshPassType::BasePass]) {
            auto pso = g_runtimeContext.psoCache->GetPso(meshDrawCommand.pipelineID);

            encoder.SetViewport(m_viewPort);
            encoder.SetScissor(0, 0, m_viewPort.width, m_viewPort.height);

            auto vertexBuffer = meshDrawCommand.drawMesh.meshSource->vertexBuffer;
            auto indexBuffer  = meshDrawCommand.drawMesh.meshSource->indexBuffer;

            encoder.BindPSO(pso);
            encoder.BindVertexBuffer(0, 1, vertexBuffer->GetNativeHandle(), 0);
            encoder.BindIndexBuffer(indexBuffer->GetNativeHandle(), 0, vk::IndexType::eUint32);

            encoder.DrawIndexed(3 * meshDrawCommand.drawMesh.meshSource->indices.size(), 1, 0, 0,
                                0);

            encoder.RenderUI();
        }
    });
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