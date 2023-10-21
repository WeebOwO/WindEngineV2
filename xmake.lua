set_project("WindVK")

add_rules("mode.debug", "mode.release")
add_includedirs("Wind/")

set_languages("c++20")
set_runtimes("MD")

includes("Editor/")
includes("Wind/")

