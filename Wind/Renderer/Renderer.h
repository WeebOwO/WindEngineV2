#pragma once

#include "Core/Service.h"
#include "PsoCache.h"


#include "Backend/Command.h"
#include "Backend/Descriptor.h"
#include "Backend/Device.h"
#include "Engine/RenderConfig.h"


#include "RenderGraph/RenderGraph.h"

// define lowlevel renderer that support the whole render system
namespace wind
{
    class Swapchain;
    class PsoCache;

    class FrameParms
    {
    public:
        Ref<CommandEncoder> renderEncoder;
        Ref<CommandEncoder> computeEncoder;

        vk::Semaphore imageAvailableSemaphore;
        vk::Semaphore renderFinishedSemaphore;
        vk::Fence     flightFence;

        uint32_t swapchainImageIndex;

        Ref<DescriptorAllocator> dynamicDescriptorAllocator; // have a list of descriptor pool

    private:
        friend class Renderer;
        void Init(vk::Device device);
        void Destroy(vk::Device device);

        void ResetCommanEncoders();
    };

    class Renderer : public Service<Renderer>
    {
    public:
        Renderer(GPUDevice& device) : m_device(device) {}
        enum class State
        {
            Idle = 0,
            Kick,
        };

        void Init() override;
        void Quit() override;

        // return the render graph will use this frame
        RenderGraph& BeginFrame(const Swapchain& swapchain);
        void         NextFrame();

        auto& GetCurrentFrameData() { return m_frameParams[m_frameNumber]; }
        auto  GetMaterialManager() const noexcept { return m_materialManager.get(); }

        template<typename... Args>
        uint64_t CachePso(Args&&... args)
        {
            return m_psoCache->CachePso(args...);
        }

        auto GetPso(uint64_t id) { return m_psoCache->GetPso(id); }

    private:
        GPUDevice&             m_device;
        uint32_t               m_frameNumber = 0;
        FrameParms             m_frameParams[RenderConfig::MAX_FRAME_IN_FLIGHT];
        Scope<MaterialManager> m_materialManager;
        Scope<RenderGraph>     m_renderGraph;
        Scope<ShaderMap>       m_shaderMap;
        Scope<PsoCache>        m_psoCache;
    };
} // namespace wind