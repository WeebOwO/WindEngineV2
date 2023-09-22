#pragma once

#include "VulkanHeader.h"

namespace wind {
// this file define render command, all command is stateless which is very easy for cache

enum class RenderCommandType : uint8_t {
    Draw = 0,
    DrawIndiret,
    DisPatch,
    DisPatchIndirect,
    Barriers,
    Count
};

enum class RenderCommandQueueType : uint8_t {
    None = 0x00,
    Copy = 0x01,
    Compute = 0x02,
    Graphics = 0x04,
    All = Copy | Compute | Graphics
};

struct RenderCommand {
    RenderCommandType type = RenderCommandType::Count;
};

template<RenderCommandType Type, RenderCommandQueueType QueueType> 
struct RenderCommandTyped : RenderCommand {
    static constexpr RenderCommandType type = Type;
    static constexpr RenderCommandQueueType queueType = QueueType;
    RenderCommandTyped() {this->type = type;}
};

struct RenderCommandDraw : public RenderCommandTyped<RenderCommandType::Draw, RenderCommandQueueType::Graphics> {
    uint32_t vertexCount;
    uint32_t instanceCount;
    uint32_t firstVertex;
    uint32_t firstInstance;
};

struct RenderCommandDispatch : public RenderCommandTyped<RenderCommandType::DisPatch, RenderCommandQueueType::Compute> {
    uint32_t dispatchX;
    uint32_t dispatchY;
    uint32_t dispatchZ;
};

} // namespace wind