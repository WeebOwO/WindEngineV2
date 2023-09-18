#pragma once

#include <string_view>
#include "GLFW/glfw3.h"

namespace wind {
class Window {
public:
    Window(uint32_t width, uint32_t height, std::string_view title);
    ~Window();

    [[nodiscard]] auto GetWindow() const { return m_window; }
    [[nodiscard]] auto width() const { return m_windowInfo.width; }
    [[nodiscard]] auto height() const { return m_windowInfo.height; }

    void OnUpdate(float fs);

private:
    struct WindowInfo {
        uint32_t    width, height;
        std::string title;
    };

    GLFWwindow* m_window{nullptr};
    WindowInfo  m_windowInfo{};
};
} // namespace wind
