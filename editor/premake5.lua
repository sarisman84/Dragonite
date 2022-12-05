include "vendor"

function initEditor(aName, aDir, aWorkingDir, aTempDir)
    local prjDir = "../editor/"
    local src = prjDir .. "src/"
    local name = "editor"
    project(name)
    location(prjDir)
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    output = "%{cfg.buildcfg}"
    solutionDir = ("%{wks.location}" .. "/../")
    prjName = "%{prj.name}"

    
    local tempDir = 
    targetdir(aDir) -- ouput dir  
    objdir(aTempDir) -- intermediate dir
    debugdir(aWorkingDir)

    -- trymkdir(aDir)
    -- trymkdir(aWorkingDir)

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    includedirs {src, src .. "/shaders/includes", "../vendor/src/", "../vendor/src/imgui/",
                 "../vendor/src/imgui/backends/"}
    fetchVendorInclude()

    links {"vendor"}
    libdirs {"../lib"}

    files {src .. "**.h", src .. "**.hpp", src .. "**.c", src .. "**.cpp", src .. "**.hlsl", src .. "**.hlsli"}

    fetchVendorInclude()

    filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "on"
    links {"vendor_Debug.lib"}

    filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "on"
    links {"vendor_Release.lib"}

    filter "configurations:Retail"
    defines "_RETAIL"
    runtime "Release"
    optimize "on"
    links {"vendor_Retail.lib"}

    return tDir
end
