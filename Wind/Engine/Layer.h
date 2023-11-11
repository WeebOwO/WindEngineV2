#pragma once

#include "std.h"

#include "Core/TimeStep.h"

namespace wind {
class Layer {
public:
    Layer(const std::string& name = "layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}

protected:
    std::string m_debugName;
};
} // namespace wind