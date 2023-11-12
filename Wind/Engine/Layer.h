#pragma once

#include "std.h"

#include "Core/Event.h"

namespace wind {
class Layer {
public:
    Layer(const std::string& name = "layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(float ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    const std::string& GetName() { return m_debugName; }

protected:
    std::string m_debugName;
};
} // namespace wind