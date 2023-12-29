#pragma once

#include "std.h"

#include "Backend/Command.h"

namespace wind {
enum class EPassType { Graphics, Compute, AsyncCompute };

class ResourceRegistry;
class RenderGraphPassBase {
public:
    RenderGraphPassBase() = default;
    virtual ~RenderGraphPassBase() {}

    virtual void Execute(ResourceRegistry&, CommandBuffer&) noexcept {}

    EPassType passType;
};

template <typename Data> 
class RenderGraphPass : public RenderGraphPassBase {
public:
    void Execute(ResourceRegistry&, CommandBuffer&) noexcept override {}
    RenderGraphPass(EPassType type) { passType = type; }
    virtual ~RenderGraphPass() {}

    const Data& GetData() const noexcept { return m_data; }
    const Data* operator->() const { return &m_data; }

protected:
    Data m_data;
};

template <typename Data, typename ExecuteCallBack>
class RenderGraphPassConcrete : public RenderGraphPass<Data> {
public:
    explicit RenderGraphPassConcrete(ExecuteCallBack&& callback,
                                     EPassType         type = EPassType::Graphics)
        : RenderGraphPass<Data>(type), m_execCallBack(std::move(callback)) {}

    void Execute(ResourceRegistry& resourceRegistry, CommandBuffer& encoder) noexcept override { 
        m_execCallBack(resourceRegistry, this->m_data, encoder); 
    }

private:
    ExecuteCallBack m_execCallBack;
};
}; // namespace wind