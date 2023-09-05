add_requires("vulkansdk", "vulkan-memory-allocator", "spdlog", "glfw", "glm", "entt", "boost")

target("WindRuntime")
    set_kind("static")
    add_files("/**.cpp")
    add_packages("vulkansdk", "vulkan-memory-allocator", "glfw", "spdlog", "glm", "boost")