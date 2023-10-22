#pragma once

#include "std.h"

#include "VulkanHeader.h"

namespace wind {
class DescriptorLayoutCache {
public:
    struct DescriptorLayoutInfo {
        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        bool                 operator==(const DescriptorLayoutInfo& other) const;
        [[nodiscard]] size_t hash() const;
    };
    vk::DescriptorSetLayout
    CreateDescriptorsetlayout(const vk::DescriptorSetLayoutCreateInfo layoutInfo);

    void Init(vk::Device);
    void CleanUp();

private:
    struct DescriptorLayoutHash {
        std::size_t operator()(const DescriptorLayoutInfo& k) const { return k.hash(); }
    };
    using LayoutCache =
        std::unordered_map<DescriptorLayoutInfo, vk::DescriptorSetLayout, DescriptorLayoutHash>;

    vk::Device  m_device;
    LayoutCache m_layoutCache;
};

class DescriptorAllocator {
public:
    struct PoolSizes {
        std::vector<std::pair<vk::DescriptorType, float>> sizes = {
            {vk::DescriptorType::eSampler, 0.5f},
            {vk::DescriptorType::eCombinedImageSampler, 4.f},
            {vk::DescriptorType::eUniformBuffer, 2.f},
            {vk::DescriptorType::eStorageBuffer, 2.f},
            {vk::DescriptorType::eSampledImage, 2.f},
            {vk::DescriptorType::eInputAttachment, 0.5f},
            {vk::DescriptorType::eUniformBufferDynamic, 0.5f},
            {vk::DescriptorType::eStorageBufferDynamic, 0.5f}};
    };

    void Init(vk::Device);
    void CleanUp();

    vk::DescriptorSet Allocate(const vk::DescriptorSetLayout& layout);

private:
    vk::Device         m_device;
    vk::DescriptorPool GrabPool();

    PoolSizes descriptorSizes;

    vk::DescriptorPool              m_currentPool{nullptr};
    std::vector<vk::DescriptorPool> m_usedPools;
    std::vector<vk::DescriptorPool> m_freePools;
};
} // namespace wind