#pragma once

#include "Core/Log.h"
#include "VulkanHeader.h"

namespace wind::utils {
inline void CheckVkResult(VkResult result) {
    if (result != VK_SUCCESS) {
        WIND_CORE_ERROR("VkResult is '{0}'");
        if (result == VK_ERROR_DEVICE_LOST) {
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(3s);
        }
    }
}
} // namespace wind::utils