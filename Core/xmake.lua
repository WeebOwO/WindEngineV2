add_requires("vulkansdk", "vulkan-memory-allocator", "spdlog", "glfw", "glm", "entt", "boost", "tracy")

option("tracy")
    set_default(true)
    set_showmenu(true)
    add_defines("TRACY_ENABLE")

target("WindRuntime")
    add_options("tracy")
    set_kind("static")
    add_files("/**.cpp")
    add_packages("vulkansdk", "vulkan-memory-allocator", "glfw", "spdlog", "glm", "boost", "tracy", "entt")