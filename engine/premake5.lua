include "vendor"
function initEngine(aName, aDir, aPostBuildEventCommand)
    local prjDir = "../engine/"
    local src = prjDir .. "src/"
    local name = "engine"
    project(name)
    location(prjDir)
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"

    output = "%{cfg.buildcfg}"
    solutionDir = ("%{wks.location}" .. "/../")
    prjName = "%{prj.name}"

    local tDir = aDir .. "/engine"
    local tempDir = tDir .. "/temp"
    targetdir(tDir) -- ouput dir  
    objdir(tempDir) -- intermediate dir
    debugdir(tDir)

    trymkdir(tDir)
    trymkdir(tempDir)

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    includedirs {
        src,
        src .. "/shaders/includes",
        "../vendor/src/"
    }

    libdirs {
        "../lib"
    }

    links {
        "vendor"
    }

    files {
        src .. "**.h",
        src .. "**.hpp",
        src .. "**.c",
        src .. "**.cpp",
        src .. "shaders/**.hlsl",
        src .. "shaders/includes/**.hlsli"
    }

    postbuildcommands {
        aPostBuildEventCommand
    }
    
    filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "on"
    links {
        "vendor_Debug.lib"
    }

    filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "on"
    links {
        "vendor_Release.lib"
    }

    filter "configurations:Retail"
    defines "_RETAIL"
    runtime "Release"
    optimize "on"
    links {
        "vendor_Retail.lib"
    }

    return tDir
end

