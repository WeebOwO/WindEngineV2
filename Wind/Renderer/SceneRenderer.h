#pragma once

#include "std.h"

#include "RenderBackend/Fwd.h"
#include "RenderBackend/Command.h"
#include "Renderbackend/SwapChain.h"

#include "RenderGraph/RenderGraph.h"

namespace wind {
struct RenderConfig {
    uint32_t MAX_FRAME_IN_FLIGHT = 2;
};

struct FrameParms {
    void Init(const vk::Device& device);
    void Destroy(const vk::Device& device);

    void ResetCommanEncoders();

    Ref<CommandEncoder> renderEncoder;
    Ref<CommandEncoder> computeEncoder;

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

    void        Render(Swapchain& swapchain);
    FrameParms& GetCurrentFrameData();

private:
    void InitView();

    void BasePassRendering();
    void PresentPass();

    GPUDevice&       m_device;
    FrameParms       m_frameParams[Swapchain::MAX_FRAME_IN_FLIGHT];
    u32              m_frameNumber{0};
    Ref<RenderGraph> m_renderGraph;
};
} // namespace wind