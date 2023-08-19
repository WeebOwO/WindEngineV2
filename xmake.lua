set_project("WindVK")

add_rules("mode.debug", "mode.release")
add_requires("vulkansdk")
add_includedirs("Core/")

set_languages("c++20")
set_runtimes("MD")

includes("Example/")

target("WindVK")
    set_kind("static")
    add_files("Source/*.cpp", "Core/*.mpp")
