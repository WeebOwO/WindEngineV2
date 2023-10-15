#pragma once

#include <GLFW/glfw3.h>

#include "std.h"

namespace wind {
class Swapchain;
class Window {
public:
    Window(uint32_t width, uint32_t height, std::string_view title);
    ~Window();

    [[nodiscard]] auto GetWindow() const { return m_window; }
    [[nodiscard]] auto width() const { return m_windowInfo.width; }
    [[nodiscard]] auto height() const { return m_windowInfo.height; }

    Swapchain* GetSwapChain() const { return m_swapchain.get(); }
    
    void       Init();
    void       OnUpdate(float fs);

private:
    struct WindowInfo {
        uint32_t    width, height;
        std::string title;
    };

    GLFWwindow*                m_window{nullptr};
    WindowInfo                 m_windowInfo{};
    Scope<Swapchain> m_swapchain;
};
} // namespace wind
