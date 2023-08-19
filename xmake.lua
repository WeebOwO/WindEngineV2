set_project("WindVK")

add_rules("mode.debug", "mode.release")
set_languages("c++20")
add_includedirs("Core")

includes("Example/Triangle/")

target("WindVK")
    set_kind("static")
    add_files("Source/*.cpp", "Core/*.mpp")
