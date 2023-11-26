#pragma once

#include "std.h"

#include "RenderGraphResource.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"

namespace wind {
class RenderGraph;
enum class EPassType { Graphics, Compute, AsyncCompute };

class RenderGraphPassBase {
public:
    RenderGraphPassBase() = default;
    virtual ~RenderGraphPassBase() {}

    virtual void Execute() noexcept {}

    EPassType passType;
};

template <typename Data> 
class RenderGraphPass : public RenderGraphPassBase {
public:
    void Execute() noexcept override {}
    RenderGraphPass(EPassType type) { passType = type; }
    virtual ~RenderGraphPass() {}

    const Data& GetData() const noexcept { return m_data; }
    const Data* operator->() const { return &m_data; }

private:
    Data m_data;
};

template <typename Data, typename ExecuteCallBack>
class RenderGraphPassConcrete : public RenderGraphPass<Data> {
public:
    explicit RenderGraphPassConcrete(ExecuteCallBack&& callback,
                                     EPassType         type = EPassType::Graphics)
        : RenderGraphPass<Data>(type), m_execCallBack(std::move(callback)) {}

    void Execute() noexcept override { m_execCallBack(); }

private:
    ExecuteCallBack m_execCallBack;
};
}; // namespace wind