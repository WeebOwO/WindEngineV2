#include "CompileContext.h"

#include "ComputeShader.h"

namespace wind {
void ComputeCompileContext::CompileCommand(const RenderCommandDispatch& command) {
    auto computerShader = command.computerShader;
    auto pipeline       = computerShader->GetPipeline();

    m_cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    m_cmdBuffer.dispatch(command.dispatchX, command.dispatchY, command.dispatchZ);
}

} // namespace wind