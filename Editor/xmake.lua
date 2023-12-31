target("WindEditor")
    set_kind("binary")
    add_files("/**.cpp")
    add_packages("vulkansdk", "glfw", "vulkan-memory-allocator", "lua")
    add_deps("WindRuntime")