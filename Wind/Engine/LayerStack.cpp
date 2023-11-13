#include "LayerStack.h"

namespace wind {
LayerStack::LayerStack() {}

LayerStack::~LayerStack() {}

void LayerStack::PushLayer(Scope<Layer> layer) {
    m_layers.emplace(m_layers.begin() + m_layerInsertIndex, std::move(layer));
    m_layerInsertIndex++;
}

void LayerStack::PushOverlay(Scope<Layer> overlay) { m_layers.emplace_back(std::move(overlay)); }

void LayerStack::PopLayer(Scope<Layer> layer) {
    auto it = std::find(m_layers.begin(), m_layers.end(), layer);
    if (it != m_layers.end()) {
        m_layers.erase(it);
        m_layerInsertIndex--;
    }
}

void LayerStack::PopOverlay(Scope<Layer> overlay) {
    auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
    if (it != m_layers.end()) m_layers.erase(it);
}
} // namespace wind