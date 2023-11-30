#include "ImGUIContext.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "Device.h"

#include "Engine/Window.h"

#include "RenderBackend/Utils.h"
#include "RenderBackend/Command.h"
#include "RenderBackend/SwapChain.h"

namespace wind {

void ImGUIContext::Init(const GPUDevice& device, const Window& window) {
    VkDevice   vkdevice  = (VkDevice)device.GetVkDeviceHandle();
    Swapchain* swapchain = window.GetSwapChain();

    VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 100},
                                         {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
                                         {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100},
                                         {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100},
                                         {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100},
                                         {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100}};

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType                      = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags                      = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets                    = 100;
    pool_info.poolSizeCount              = std::size(pool_sizes);
    pool_info.pPoolSizes                 = pool_sizes;

    vkCreateDescriptorPool(vkdevice, &pool_info, nullptr, &m_imguiPool);

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
     // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForVulkan(window.GetWindow(), true);

    ImGui_ImplVulkan_InitInfo initInfo;
    initInfo.Instance              = device.GetVkInstance();
    initInfo.PhysicalDevice        = device.GetVkPhysicalDevice();
    initInfo.Device                = vkdevice;
    initInfo.Queue                 = device.GetGraphicsQueue();
    initInfo.QueueFamily           = device.GetQueueIndices().graphicsQueueIndex.value();
    initInfo.PipelineCache         = nullptr;
    initInfo.DescriptorPool        = m_imguiPool;
    initInfo.MinImageCount         = 3;
    initInfo.ImageCount            = 3;
    initInfo.Allocator             = nullptr;
    initInfo.MSAASamples           = VK_SAMPLE_COUNT_1_BIT;
    initInfo.ColorAttachmentFormat = VkFormat(window.GetSwapChain()->GetFormat());
    initInfo.CheckVkResultFn       = CheckVkResult;
    initInfo.UseDynamicRendering   = true;

    ImGui_ImplVulkan_Init(&initInfo, nullptr);

    ImmCommandEncoder taskEncoder;
    taskEncoder.PushTask([](const vk::CommandBuffer& buffer) {
        VkCommandBuffer Cbuffer = (VkCommandBuffer)buffer;
        ImGui_ImplVulkan_CreateFontsTexture(Cbuffer);
    });

    ImGui_ImplVulkan_DestroyFontUploadObjects();
    taskEncoder.Submit();
}

void ImGUIContext::Quit(const GPUDevice& device) {
    VkDevice vkdevice = (VkDevice)device.GetVkDeviceHandle();
    vkDestroyDescriptorPool(vkdevice, m_imguiPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
}
} // namespace wind