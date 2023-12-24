#pragma once

#include "Engine/RenderConfig.h"
#include "Backend/Command.h"
#include "Backend/Descriptor.h"

namespace wind {
class SceneRenderer;
class RenderGraph;
class Swapchain;

class FrameParms {
public:
    Ref<CommandEncoder> renderEncoder;
    Ref<CommandEncoder> computeEncoder;

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

    RenderGraph& BeginFrame(const Swapchain& swapchain);
    void         NextFrame();

    auto& GetCurrentFrameData() { return m_frameParams[m_frameNumber]; }

private:
    FrameParms           m_frameParams[RenderConfig::MAX_FRAME_IN_FLIGHT];
    u32                  m_frameNumber = 0;
    Scope<RenderGraph>   m_renderGraph;
};
} // namespace wind