set_project("WindVK")

add_rules("mode.debug", "mode.release")

add_includedirs("Wind/")
add_includedirs("3rdParty/Imgui")

set_languages("c++20")
set_runtimes("MD")

includes("Editor/")
includes("Wind/")