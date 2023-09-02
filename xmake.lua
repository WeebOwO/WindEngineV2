set_project("WindVK")

add_rules("mode.debug", "mode.release")
add_includedirs("Core/")

set_languages("c++23")
set_runtimes("MD")

includes("Editor/")
includes("Core/")

