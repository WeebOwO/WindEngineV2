function main (target) 
    build_path = ""
    if is_mode("release") then
        build_path = "$(buildir)/windows/x64/release/"
    else
        build_path = "$(buildir)/windows/x64/debug/"
    end
        os.cp("Shaders/CompiledShader/**.spv", build_path)
end