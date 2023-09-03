#pragma once

namespace wind {
enum class BoundingBoxType {
    AABB = 0,
    OBB,
    Sphere   
};

template<BoundingBoxType Type>
struct BoundingBox {
    static constexpr BoundingBoxType type = Type;
};
} // namespace wind