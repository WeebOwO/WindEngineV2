#include "Command.h"

#include "ComputeShader.h"

namespace wind {
ComputeEncoder::ComputeEncoder(bool isAsync)
    : CommandEncoder(isAsync ? RenderCommandQueueType::AsyncCompute
                             : RenderCommandQueueType::Compute) {}

void ComputeEncoder::Dispatch(u32 x, u32 y, u32 z) { m_nativeHandle.dispatch(x, y, z); }

void ComputeEncoder::BindComputShader(const ComputeShader& computeShader) {
    computeShader.BindCommandBuffer(m_nativeHandle);
}
} // namespace wind