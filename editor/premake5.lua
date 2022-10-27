include "vendor"

function initEditor(aName, targetDir, debugDir)
    local prjDir = "../editor/"
    local src = prjDir .. "src/"
    local name = "editor" .. aName
    project(name)
    location(prjDir)
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    output = "%{cfg.buildcfg}"
    solutionDir = ("%{wks.location}" .. "/../")
    prjName = "%{prj.name}"

    targetdir(targetDir .. "/engine") -- ouput dir  
    objdir(targetDir .. "/engine") -- intermediate dir
    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name
    local workDir = debugDir .. "/editor"
    debugdir(workDir)

    includedirs {
        src,
        src .. "/shaders/includes",
        fetchVendorInclude("imgui", workDir),
        fetchVendorInclude("imguizmo", workDir)
    }

    

    files 
    {
        src .. "**.h",
        src .. "**.hpp",
        src .. "**.c",
        src .. "**.cpp",
        src .. "shaders/**.hlsl",
        src .. "shaders/includes/**.hlsli"
    }

end