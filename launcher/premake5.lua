workspace "Application"
location "."
architecture "x64"
startproject "game_launcher"

configurations {
    "Debug",
    "Release",
    "Retail"
}

function setLinks(someLinks)
    links = someLinks
end
-- include "vendor"
include "engine"
include "editor"
function initLauncher(aName)
    local name = aName
    project(name)
    location "."
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"

    output = "%{cfg.buildcfg}"
    solutionDir = ("%{wks.location}" .. "/../")
    prjName = "%{prj.name}"

    local tDir = solutionDir .. "bin/" .. prjName .. "/"
    local dDir = "../bin/resc/launcher"

    targetdir(tDir) -- ouput dir  

    objdir(solutionDir .. "temp/" .. prjName) -- intermediate dir
    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    debugdir(dDir)

    flags {
        "MultiProcessorCompile"
    }

    if (string.find(name, "editor")) then
        links {
            "engine",
            "editor"
        }
    else
        links {
            "engine"
        }
    end

    includedirs {
        "src/"
    }

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp"
    }

    filter "configurations:Debug"
    defines "_DEBUG"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
    defines "_RELEASE"
    runtime "Release"
    optimize "on"

    filter "configurations:Retail"
    defines "_RETAIL"
    runtime "Release"
    optimize "on"

    if(string.find(name, "editor")) then
        initEditor(name, tDir, dDir)
    else
        initEngine(name, tDir, dDir)
    end
    
end

initLauncher("game_launcher")
initLauncher("editor_launcher")

