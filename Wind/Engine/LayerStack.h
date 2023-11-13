#pragma once

#include "Engine/Layer.h"
#include "Layer.h"

namespace wind {
class LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Scope<Layer> layer);
    void PushOverlay(Scope<Layer> overlay);
    void PopLayer(Scope<Layer> layer);
    void PopOverlay(Scope<Layer> overlay);

    Layer* operator[](size_t index) {
        assert(index >= 0 && index < m_layers.size());
        return m_layers[index].get();
    }

    const Layer* operator[](size_t index) const {
        assert(index >= 0 && index < m_layers.size());
        return m_layers[index].get();
    }

    size_t Size() const { return m_layers.size(); }

    std::vector<Scope<Layer>>::iterator begin() { return m_layers.begin(); }
    std::vector<Scope<Layer>>::iterator end() { return m_layers.end(); }

private:
    std::vector<Scope<Layer>> m_layers;
    unsigned int              m_layerInsertIndex = 0;
};
} // namespace wind