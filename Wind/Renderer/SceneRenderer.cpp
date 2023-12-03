#include "SceneRenderer.h"

#include <imgui.h>

#include "View.h"
#include "MeshPass.h"

#include "RenderGraph/ResourceRegistry.h"
#include "RenderGraph/RenderGraphResource.h"
#include "RenderGraph/RenderGraphTexture.h"
#include "RenderGraph/RenderPassEnum.h"

#include "Core/Log.h"

#include "Engine/RuntimeContext.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/ComputeShader.h"
#include "RenderBackend/Descriptor.h"

#include "Resource/Loader.h"
#include "Resource/Mesh.h"
#include "Resource/VertexFactory.h"

#include "Scene/Scene.h"

namespace wind {

void SceneRenderer::Init() {}

void SceneRenderer::InitView(View& view) {
    for (auto meshPassType = MeshPassType::BasePass; meshPassType != MeshPassType::Count;
         meshPassType      = Step(meshPassType)) {
        BuildMeshDrawCommand(m_renderScene->m_meshPasses[meshPassType]);
    }
}

void SceneRenderer::SetViewPort(float offsetX, float offsetY, float width, float height) {
    m_viewPortWidth  = u32(width);
    m_viewPortHeight = u32(height);

    m_viewPort.setWidth(width)
        .setHeight(height)
        .setX(offsetX)
        .setY(offsetY)
        .setMinDepth(0.0)
        .setMaxDepth(1.0);
}

void SceneRenderer::Render(View& view, RenderGraph& rg) {
    m_renderScene = RuntimeUtils::GetActiveScene();
    InitView(view);

    struct ColorPassData {
        RenderGraphID<RenderGraphTexture> sceneColor;
    };

    vk::ClearValue clearValue {
        .color = vk::ClearColorValue{.float32 = std::array<float, 4>{1.0f, 1.0f, 1.0f, 1.0f}}
    };

    vk::Rect2D renderArea = {
        .offset = {.x = 0, .y = 0},
        .extent = {.width = m_viewPortWidth, .height = m_viewPortHeight}
    };

    if(!rg.ContainPass("LightingPass")) {
        auto& colorPass = rg.AddPass<ColorPassData>(
        "LightingPass",
        [&](RenderGraph::Builder& builder, ColorPassData& data) {
            data.sceneColor = builder.CreateTexture(
                "SceneColor", utils::GetRenderTargetDesc(m_viewPortWidth, m_viewPortHeight, vk::Format::eR16G16B16A16Sfloat));

            RenderPassNode::RenderDesc renderDesc {
                .attchments = {.color = {data.sceneColor}, .depth = {}, .stencil = {}},
                .renderArea = renderArea,
                .sample = 1,
                .clearValue = clearValue,
            };

            builder.DeclareRenderPass(renderDesc);
        },
        [&](ResourceRegistry& resourceRegistry, ColorPassData& data, CommandEncoder& encoder) {
            encoder.BeginRendering(resourceRegistry.GetRenderingInfo());
            for(const auto meshDrawCommand : m_cacheMeshDrawCommands[BasePass]) {
                encoder.DrawMesh(meshDrawCommand);
            }   
            encoder.EndRendering();
        }, EPassType::Graphics);
    }
    
    return;
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