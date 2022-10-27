function initEngine(aName, targetDir, debugDir)
    local prjDir = "../engine/"
    local src = prjDir .. "src/"
    local name = "engine_" .. aName
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

    debugdir(debugDir .. "/engine")

    includedirs {
        src,
        src .. "/shaders/includes"
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

