add_requires("vulkansdk", "vulkan-memory-allocator", "spdlog", "glfw", "glm", "entt", "tracy", "spirv-cross", "shaderc")

option("tracy")
    set_default(true)
    set_showmenu(true)
    add_defines("TRACY_ENABLE")
    
target("WindRuntime")
    before_build("before_build")
    after_build("after_build")
    set_pcxxheader("std.h")
    add_options("tracy")
    set_kind("static")
    add_files("/**.cpp", "../3rdParty/Imgui/**.cpp")
    add_packages("vulkansdk", "vulkan-memory-allocator", "glfw", "spdlog", "glm", "tracy", "entt", "spirv-cross", "shaderc")   