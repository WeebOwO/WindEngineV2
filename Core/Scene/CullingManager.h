#pragma once

namespace wind {
enum class CullingMethod {
    None = 0,
    Normal,  // do traditional cpu culling
    Compute, // using Compute shader culling
};

class CullingManager {
    
};

}