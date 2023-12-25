#include "Descriptor.h"

#include "Device.h"

namespace wind {
bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==(
    const DescriptorLayoutInfo& other) const {
    if (other.bindings.size() != bindings.size()) {
        return false;
    } else {
        for (int i = 0; i < bindings.size(); ++i) {
            if (other.bindings[i].binding != bindings[i].binding) {
                return false;
            } else if (other.bindings[i].descriptorType != bindings[i].descriptorType) {
                return false;
            } else if (other.bindings[i].descriptorCount != bindings[i].descriptorCount) {
                return false;
            } else if (other.bindings[i].stageFlags != bindings[i].stageFlags) {
                return false;
            }
        }
        return true;
    }
}

size_t DescriptorLayoutCache::DescriptorLayoutInfo::hash() const {
    using std::hash;
    using std::size_t;

    size_t result = hash<size_t>{}(bindings.size());

    for (const auto& b : bindings) {
        size_t bindHash = b.binding | (uint32_t)b.descriptorType << 8 |
                          (uint32_t)b.descriptorCount << 16 | (uint32_t)b.stageFlags << 24;
        result ^= hash<size_t>{}(bindHash);
    }

    return result;
}

void DescriptorLayoutCache::Init(vk::Device device) { m_device = device; };

vk::DescriptorSetLayout DescriptorLayoutCache::CreateDescriptorsetlayout(
    const vk::DescriptorSetLayoutCreateInfo layoutInfo) {
    DescriptorLayoutInfo ownLayoutInfo;
    uint32_t             bindCount = layoutInfo.bindingCount;

    ownLayoutInfo.bindings.reserve(bindCount);
    bool isSorted    = false;
    int  lastBinding = -1;

    for (uint32_t i = 0; i < bindCount; ++i) {
        ownLayoutInfo.bindings.push_back(layoutInfo.pBindings[i]);
        if (layoutInfo.pBindings[i].binding > lastBinding) {
            lastBinding = layoutInfo.pBindings[i].binding;
        } else {
            isSorted = false;
        }
    }

    if (!isSorted) {
        // sort our binding
        std::sort(ownLayoutInfo.bindings.begin(), ownLayoutInfo.bindings.end(),
                  [](const vk::DescriptorSetLayoutBinding& a,
                     const vk::DescriptorSetLayoutBinding& b) { return a.binding < b.binding; });
    }

    auto it = m_layoutCache.find(ownLayoutInfo);
    if (it != m_layoutCache.end()) {
        return it->second;
    } else {
        vk::DescriptorSetLayout retLayout = m_device.createDescriptorSetLayout(layoutInfo);
        m_layoutCache[ownLayoutInfo]      = retLayout;
        return retLayout;
    }
}

void DescriptorLayoutCache::CleanUp() {
    for (auto& [key, value] : m_layoutCache) {
        m_device.destroyDescriptorSetLayout(value);
    }
}

vk::DescriptorPool CreatePool(vk::Device device, const DescriptorAllocator::PoolSizes& poolSizes,
                              int count) {
    std::vector<vk::DescriptorPoolSize> sizes;
    sizes.reserve(poolSizes.sizes.size());

    for (const auto sz : poolSizes.sizes) {
        sizes.push_back({sz.first, uint32_t(sz.second * count)});
    }

    vk::DescriptorPoolCreateInfo createInfo;
    createInfo.setPoolSizeCount(sizes.size()).setPoolSizes(sizes).setMaxSets(count);

    return device.createDescriptorPool(createInfo);
}

DescriptorAllocator::DescriptorAllocator(const vk::Device& device, uint32_t descriptorBaseNum)
    : m_device(device), m_descriptorBaseNum(descriptorBaseNum) {}

DescriptorAllocator::~DescriptorAllocator() {
    for (auto p : m_freePools) {
        m_device.destroyDescriptorPool(p);
    }
    for (auto p : m_usedPools) {
        m_device.destroyDescriptorPool(p);
    }
}

vk::DescriptorPool DescriptorAllocator::GrabPool() {
    if (m_freePools.size() > 0) {
        vk::DescriptorPool pool = m_freePools.back();
        m_freePools.pop_back();
        return pool;
    } else {
        return CreatePool(m_device, descriptorSizes, 1000);
    }
}

vk::DescriptorSet DescriptorAllocator::Allocate(const vk::DescriptorSetLayout& layout) {
    if (!m_currentPool) {
        m_currentPool = GrabPool();
        m_usedPools.push_back(m_currentPool);
    }

    vk::DescriptorSetAllocateInfo allcateInfo{
        .descriptorPool = m_currentPool, .descriptorSetCount = 1, .pSetLayouts = &layout};

    return m_device.allocateDescriptorSets(allcateInfo).front();
}
} // namespace wind