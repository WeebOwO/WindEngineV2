#pragma once

#include "std.h"

#include "RenderBackend/Command.h"
#include "RenderBackend/Texture.h"
#include "RenderGraphResource.h"

namespace wind {
class RenderGraph;
enum class EPassType { Graphics, Compute, AsyncCompute };

class RenderGraphPassExecutor {
protected:
    virtual void Execute() noexcept = 0;

public:
    RenderGraphPassExecutor() noexcept = default;
    virtual ~RenderGraphPassExecutor() noexcept;
    RenderGraphPassExecutor(RenderGraphPassExecutor const&)            = delete;
    RenderGraphPassExecutor& operator=(RenderGraphPassExecutor const&) = delete;
};

class RenderGraphPassBase {
public:
    RenderGraphPassBase() = default;
    virtual ~RenderGraphPassBase() {}

    virtual void Execute() {}

    EPassType passType;
};

template <typename Data> class RenderGraphPass : public RenderGraphPassBase {
public:
    void Execute() noexcept override {}
    RenderGraphPass(EPassType type) { passType = type; }
    virtual ~RenderGraphPass() {}

    Data const& GetData() const noexcept { return m_data; }
    Data const* operator->() const { return &m_data; }

private:
    Data m_data;
};

template <typename Data, typename ExecuteCallBack>
class RenderGraphPassConcrete : public RenderGraphPass<Data> {
public:
    explicit RenderGraphPassConcrete(EPassType type, ExecuteCallBack&& callback)
        : RenderGraphPass<Data>(type), m_execCallBack(std::move(callback)) {}

    void Execute() noexcept override { m_execCallBack(); }

private:
    ExecuteCallBack m_execCallBack;
};
}; // namespace wind