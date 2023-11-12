#pragma once

#include "Layer.h"

namespace wind {
class LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    Layer* operator[](size_t index) {
        assert(index >= 0 && index < m_layers.size());
        return m_layers[index];
    }

    const Layer* operator[](size_t index) const {
        assert(index >= 0 && index < m_layers.size());
        return m_layers[index];
    }

    size_t Size() const { return m_layers.size(); }

    std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_layers.end(); }

private:
    std::vector<Layer*> m_layers;
    unsigned int        m_layerInsertIndex = 0;
};
} // namespace wind