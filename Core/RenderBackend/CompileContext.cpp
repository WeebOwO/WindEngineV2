#include "CompileContext.h"

#include "ComputeShader.h"

namespace wind {
    vk::PipelineBindPoint GetBindPoint(const RenderCommandQueueType& queueType) {
        if(queueType == RenderCommandQueueType::Graphics) {
            return vk::PipelineBindPoint::eGraphics;
        }    
        
        return vk::PipelineBindPoint::eCompute;
    }

    void RenderCompileContext::CompileCommand(const RenderCommandDispatch& command) {
        auto computerShader = command.computerShader;
        auto pipeline = computerShader->GetPipeline();

        m_cmdBuffer.bindPipeline(GetBindPoint(command.queueType), pipeline);
        m_cmdBuffer.dispatch(command.dispatchX, command.dispatchY, command.dispatchZ);
    }
}