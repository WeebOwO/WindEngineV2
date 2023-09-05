#pragma once

namespace wind {
template <typename Service> class Locator {
public:
    static void Provide(Service* service) {
        m_provider = service;
    }

    Service* GetService() const {
        return m_provider;
    }

private:
    static Service* m_provider;
};

template<typename Service>
Service* Locator<Service>::m_provider = nullptr;
} // namespace wind