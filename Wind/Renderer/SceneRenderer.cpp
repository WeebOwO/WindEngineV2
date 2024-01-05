#include "SceneRenderer.h"

#include <Imgui/imgui.h>

#include "MeshPass.h"
#include "Renderer.h"
#include "View.h"

#include "RenderGraph/RenderGraphPass.h"
#include "RenderGraph/RenderGraphResource.h"
#include "RenderGraph/RenderGraphTexture.h"
#include "RenderGraph/RenderPassEnum.h"
#include "RenderGraph/ResourceRegistry.h"

#include "Engine/RuntimeContext.h"

#include "Resource/Loader.h"
#include "Resource/Mesh.h"
#include "Resource/VertexFactory.h"

#include "Scene/Scene.h"

namespace wind
{

    void SceneRenderer::Init() {}

    void SceneRenderer::InitView(View& view)
    {
        for (auto meshPassType = MeshPassType::BasePass; meshPassType != MeshPassType::Count;
             meshPassType      = Step(meshPassType))
        {
            BuildMeshDrawCommand(m_renderScene->m_meshPasses[meshPassType]);
        }
    }

    void SceneRenderer::SetViewPort(float offsetX, float offsetY, float width, float height)
    {
        m_viewPortWidth  = uint32_t(width);
        m_viewPortHeight = uint32_t(height);

        m_viewPort.setWidth(width).setHeight(height).setX(offsetX).setY(offsetY).setMinDepth(0.0).setMaxDepth(1.0);
    }

    void SceneRenderer::DrawMesh(CommandBuffer& encoder)
    {
        auto renderer = g_runtimeContext.renderer.get();
        for (const auto& meshDrawCommand : m_cacheMeshDrawCommands[BasePass])
        {
            auto pso = renderer->GetPso(meshDrawCommand.pipelineID);

            encoder.SetViewport(m_viewPort);
            encoder.SetScissor(0, 0, m_viewPort.width, m_viewPort.height);

            auto vertexBuffer = meshDrawCommand.drawMesh.meshSource->vertexBuffer;
            auto indexBuffer  = meshDrawCommand.drawMesh.meshSource->indexBuffer;

            encoder.BindPSO(pso);
            encoder.BindVertexBuffer(0, 1, vertexBuffer->GetNativeHandle(), 0);
            encoder.BindIndexBuffer(indexBuffer->GetNativeHandle(), 0, vk::IndexType::eUint32);

            encoder.DrawIndexed(3 * meshDrawCommand.drawMesh.meshSource->indices.size(), 1, 0, 0, 0);
        }
    }

    void SceneRenderer::Render(View& view, RenderGraph& rg)
    {
        m_renderScene = g_runtimeContext.activeScene;
        InitView(view);

        auto& blackBoard = rg.GetBlackBoard();

        struct ColorPassData
        {
            RenderGraphID<RenderGraphTexture> sceneColor;
        };

        vk::ClearValue clearValue {.color =
                                       vk::ClearColorValue {.float32 = std::array<float, 4> {1.0f, 1.0f, 1.0f, 1.0f}}};

        vk::Rect2D renderArea = {.offset = {.x = 0, .y = 0},
                                 .extent = {.width = m_viewPortWidth, .height = m_viewPortHeight}};

        if (!rg.ContainPass("LightingPass"))
        {
            auto& colorPass = rg.AddPass<ColorPassData>(
                "LightingPass",
                [&](RenderGraph::Builder& builder, ColorPassData& data) {

                    auto colorDesc = utils::GetRenderTargetDesc(m_viewPortWidth, m_viewPortHeight, vk::Format::eR16G16B16A16Sfloat);
                    colorDesc.usage |= vk::ImageUsageFlagBits::eSampled;
                    data.sceneColor = builder.CreateTexture("SceneColor", colorDesc);

                    RenderPassNode::RenderDesc renderDesc {
                        .attchments = {.color = {data.sceneColor}, .depth = {}, .stencil = {}},
                        .renderArea = renderArea,
                        .sample     = 1,
                        .clearValue = clearValue,
                    };

                    builder.DeclareRenderPass(renderDesc);
                },
                [&](ResourceRegistry& resourceRegistry, ColorPassData& data, CommandBuffer& encoder) {
                    encoder.BeginRendering(resourceRegistry.GetRenderingInfo());
                    DrawMesh(encoder);
                    encoder.EndRendering();
                },
                PassType::Graphics);

            blackBoard["output"] = colorPass->sceneColor;
        }
    }

    void SceneRenderer::BuildMeshDrawCommand(const MeshPass& meshPass)
    {
        auto                renderer = g_runtimeContext.renderer.get();
        RenderGraphPassType graphPassType =
            meshPass.type == MeshPassType::BasePass ? RenderGraphPassType::MeshPassMRT : RenderGraphPassType::MeshPass;
        m_cacheMeshDrawCommands[meshPass.type].clear();

        for (auto meshProxy : meshPass.staticMeshes)
        {
            MeshDrawCommand meshDrawCommand;
            meshDrawCommand.drawMesh.firstVertex = 0;
            meshDrawCommand.drawMesh.meshSource  = &meshProxy->meshSource;
            meshDrawCommand.drawMesh.vertexCount = meshProxy->meshSource.vertices.size();
            meshDrawCommand.drawMesh.indexCount  = meshProxy->meshSource.indices.size();
            meshDrawCommand.materialProxy        = meshProxy->material;

            meshDrawCommand.pipelineID =
                renderer->CachePso(*meshProxy->material, VertexFactoryType::StaicMesh, graphPassType);

            m_cacheMeshDrawCommands[meshPass.type].push_back(meshDrawCommand);
        }
    }

} // namespace wind