#pragma once

#include "Engine/LayerStack.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/Descriptor.h"

namespace wind {
class SceneRenderer;
class RenderGraph;

class FrameParms {
public:
    Ref<RenderEncoder>  renderEncoder;
    Ref<ComputeEncoder> computeEncoder;

    vk::Semaphore imageAvailableSemaphore;
    vk::Semaphore renderFinishedSemaphore;
    vk::Fence     flightFence;

    u32 swapchainImageIndex;

    Ref<DescriptorAllocator> dynamicDescriptorAllocator; // have a list of descriptor pool

private:
    friend class RenderThread;
    void Init();
    void Destroy();

    void ResetCommanEncoders();
};

class RenderThread {
public:
    enum class State {
        Idle = 0,
        Kick,
    };
    
    void Init();
    void Quit();

    void RenderJob(const Swapchain& swapchain);
    void NextFrame();

    auto& GetCurrentFrameData() { return m_frameParams[m_frameNumber]; }

private:
    static constexpr u32 MAX_FRAME_IN_FLIGHT = 2;

    FrameParms           m_frameParams[MAX_FRAME_IN_FLIGHT];
    u32                  m_frameNumber = 0;
    Scope<SceneRenderer> m_sceneRenderer;
    Scope<RenderGraph>   m_renderGraph;
};
} // namespace wind