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

void FrameParms::Init(const vk::Device& device) {
    computeEncoder = ref::Create<ComputeEncoder>();
    renderEncoder  = ref::Create<RenderEncoder>();

    vk::FenceCreateInfo fenceCreateInfo{.flags = vk::FenceCreateFlagBits::eSignaled};
    flightFence = device.createFence(fenceCreateInfo);

    imageAvailableSemaphore = device.createSemaphore({});
    renderFinishedSemaphore = device.createSemaphore({});

    dynamicDescriptorAllocator = ref::Create<DescriptorAllocator>(device);
}

void FrameParms::Destroy(const vk::Device& device) {
    device.destroyFence(flightFence);
    device.destroySemaphore(imageAvailableSemaphore);
    device.destroySemaphore(renderFinishedSemaphore);
}

void FrameParms::ResetCommanEncoders() {
    computeEncoder->Reset();
    renderEncoder->Reset();
}

SceneRenderer::~SceneRenderer() {
    m_device.GetVkDeviceHandle().waitIdle();
    for (auto& frameData : m_frameParams) {
        frameData.Destroy(m_device.GetVkDeviceHandle());
    }
}

SceneRenderer::SceneRenderer() : m_device(*g_runtimeContext.device) {}

void SceneRenderer::Init() {
    auto vkDevice = m_device.GetVkDeviceHandle();
    m_renderGraph = ref::Create<RenderGraph>(m_device);
    // create frame parms
    for (auto& frameParms : m_frameParams) {
        frameParms.Init(vkDevice);
    }
    // setup present pass
    m_Presentpass = m_renderGraph->AddPass("PresentPass", RenderCommandQueueType::Graphics);
    m_Presentpass->MarkWriteBackBuffer();
}

FrameParms& SceneRenderer::GetCurrentFrameData() { return m_frameParams[m_frameNumber]; }

void SceneRenderer::SetScene(Scene& scene) { m_renderScene = &scene; }

void SceneRenderer::InitView(View& view) {

    for (auto meshPassType = MeshPassType::BasePass; meshPassType != MeshPassType::Count;
         meshPassType      = Step(meshPassType)) {
        BuildMeshDrawCommand(m_renderScene->m_meshPasses[meshPassType]);
    }
}

void SceneRenderer::Render(Swapchain& swapchain, View& view) {
    auto& frameData = GetCurrentFrameData();

    frameData.swapchainImageIndex =
        swapchain.AcquireNextImage(frameData.flightFence, frameData.imageAvailableSemaphore)
            .value();
    frameData.ResetCommanEncoders();

    m_viewPortWidth  = swapchain.GetWidth();
    m_viewPortHeight = swapchain.GetHeight();

    // init render graph
    m_renderGraph->SetupSwapChain(swapchain);
    m_renderGraph->SetupFrameData(frameData);

    InitView(view);

    PresentPass();

    m_renderGraph->Exec();
    m_frameNumber = (m_frameNumber + 1) % Swapchain::MAX_FRAME_IN_FLIGHT;
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

void SceneRenderer::PresentPass() {
    // setup present pass
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
}
} // namespace wind