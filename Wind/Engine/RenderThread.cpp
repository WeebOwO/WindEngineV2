#include "RenderThread.h"

#include "RuntimeContext.h"

#include "Backend/Command.h"
#include "Backend/SwapChain.h"

#include "Renderer/RenderGraph/RenderGraph.h"
#include "Renderer/RenderGraph/ResourceNode.h"
#include "Renderer/SceneRenderer.h"
#include "Renderer/View.h"


namespace wind
{

    void FrameParms::Init()
    {
        auto device = RuntimeUtils::GetVulkanDevice();

        computeEncoder = ref::Create<CommandEncoder>();
        renderEncoder  = ref::Create<CommandEncoder>();

        vk::FenceCreateInfo fenceCreateInfo {.flags = vk::FenceCreateFlagBits::eSignaled};
        flightFence = device.createFence(fenceCreateInfo);

        imageAvailableSemaphore = device.createSemaphore({});
        renderFinishedSemaphore = device.createSemaphore({});

        dynamicDescriptorAllocator = ref::Create<DescriptorAllocator>(device);
    }

    void FrameParms::Destroy()
    {
        auto device = RuntimeUtils::GetVulkanDevice();
        device.destroyFence(flightFence);
        device.destroySemaphore(imageAvailableSemaphore);
        device.destroySemaphore(renderFinishedSemaphore);
    }

    void FrameParms::ResetCommanEncoders()
    {
        computeEncoder->Reset();
        renderEncoder->Reset();
    }

    void RenderThread::Init()
    {
        for (auto& data : m_frameParams)
        {
            data.Init();
        }
        m_renderGraph = scope::Create<RenderGraph>();
    }

    void RenderThread::Quit()
    {
        auto device = RuntimeUtils::GetVulkanDevice();
        device.waitIdle();
        for (auto& data : m_frameParams)
        {
            data.Destroy();
        }
    }

    RenderGraph& RenderThread::BeginFrame(const Swapchain& swapchain)
    {
        auto& frameData = GetCurrentFrameData();

        frameData.swapchainImageIndex =
            swapchain.AcquireNextImage(frameData.flightFence, frameData.imageAvailableSemaphore).value();
        frameData.ResetCommanEncoders();

        m_renderGraph->SetupSwapChain(swapchain);
        m_renderGraph->SetupFrameData(frameData);

        return *m_renderGraph;
    }

    void RenderThread::NextFrame()
    {
        m_renderGraph->Exec();
        m_frameNumber = (m_frameNumber + 1) % RenderConfig::MAX_FRAME_IN_FLIGHT;
    }
} // namespace wind