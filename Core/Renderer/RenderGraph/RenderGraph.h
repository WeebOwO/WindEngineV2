#pragma once

#include "RenderBackend/VulkanHeader.h"

namespace wind {
class RenderGraph {
public:
    void Setup();
    void Compile();
    void Exec();

    void AddPass();
    
private:
    
};
} // namespace wind