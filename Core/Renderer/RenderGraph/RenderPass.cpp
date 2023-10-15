#pragma once

#include "RenderPass.h"

namespace wind {
void RenderGraphPass::SetExecCallBack(const ExecCallBack& callback) { m_execCallback = callback; }
void RenderGraphPass::SetStencilClearCallBack(const StencilClearCallBack& callback) {
    m_stencilCallback = callback;
}
void RenderGraphPass::SetRenderColorClearCallBack(const RenderColorClearCallBack& callback) {
    m_renderColorClearCallBack = callback;
}

void RenderGraphPass::Compile() {
    
}
} // namespace wind