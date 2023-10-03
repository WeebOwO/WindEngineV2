#include "Window.h"

#include "RenderBackend/Backend.h"
#include "RenderBackend/SwapChain.h"

namespace wind {

Window::Window(uint32_t width, uint32_t height, std::string_view title): m_windowInfo(WindowInfo{width, height, std::string(title)}) {
    // init the glfw context
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
}

void Window::Init() {
    auto& Device = Backend::GetGPUDevice();
    m_swapchain = std::make_unique<Swapchain>(Device, *this);
}

Window::~Window() { glfwDestroyWindow(m_window); }

void Window::OnUpdate(float fs) {
    glfwPollEvents();
}
} // namespace wind 