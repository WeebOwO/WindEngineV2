#pragma once

#include "Renderer/RenderGraph/RenderPass.h"
#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Fwd.h"
#include "Renderbackend/SwapChain.h"

#include "RenderGraph/RenderGraph.h"

namespace wind {
class Scene;
class View;

struct RenderConfig {
    uint32_t MAX_FRAME_IN_FLIGHT = 2;
};

struct FrameParms {
    void Init(const vk::Device& device);
    void Destroy(const vk::Device& device);

    void ResetCommanEncoders();

    Ref<RenderEncoder>  renderEncoder;
    Ref<ComputeEncoder> computeEncoder;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;
    vk::Fence     flightFence;

    u32 swapchainImageIndex;

    Ref<DescriptorAllocator> dynamicDescriptorAllocator; // have a list of descriptor pool
};

class SceneRenderer {
public:
    SceneRenderer();
    ~SceneRenderer();

    void        SetScene(Scene& scene);
    void        Render(Swapchain& swapchain, Scene& scene, View& view);
    FrameParms& GetCurrentFrameData();

    void Init();

private:
    void InitView();

    void PresentPass();
    
    Scene*           m_renderScene;
    GPUDevice&       m_device;
    FrameParms       m_frameParams[Swapchain::MAX_FRAME_IN_FLIGHT];
    u32              m_frameNumber{0};
    Ref<RenderGraph> m_renderGraph;

    // all the renderpass are create by rendergraph
    RenderGraphPass* m_Presentpass;
};
} // namespace wind