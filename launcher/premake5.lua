function setLinks(someLinks)
    links = someLinks
end
-- include "vendor"
include "engine"
include "editor"

function initLauncher(aName)
    project(aName)
    location "."
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"

    output = "%{cfg.buildcfg}"
    solutionDir = ("%{wks.location}" .. "/../")
    prjName = "%{prj.name}"

    local name = aName
    local binDir = os.realpath(solutionDir .. "../bin/resc")
    local launcherDir = os.realpath(binDir .. "/launcher")
    local outputDir = launcherDir .. "/build"
    local debugDir = launcherDir
    local tempDir = outputDir .. "/temp"

    targetdir(outputDir) -- ouput dir  
    if not (os.isdir(outputDir)) then
        os.mkdir(os.realpath(outputDir))
    end

    objdir(tempDir) -- intermediate dir
    if not (os.isdir(tempDir)) then
        os.mkdir(os.realpath(tempDir))
    end

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    debugdir(debugDir)

    flags {
        "MultiProcessorCompile"
    }

    links {
        "engine",
        "editor"
    }

    libdirs {
        "../lib"
    }

    includedirs {
        "src/"
    }

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp"
    }

    outputDir = os.realpath(outputDir)

    local gameDir = os.realpath(launcherDir .. "/engine/")
    local editorDir = os.realpath(launcherDir .. "/editor/")

    local gameDirCpy = "xcopy " .. outputDir .. " " .. gameDir .. " /t /e /i /y "
    local editorDirCpy = "xcopy " .. outputDir .. " " .. editorDir .. " /t /e /i /y "
    local gameToEditorDirCpy = "xcopy " .. gameDir .. " " .. editorDir .. " /y "
    postbuildcommands {
        gameDirCpy .. " && " .. editorDirCpy .. " && " .. gameToEditorDirCpy
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

    initEngine(name, launcherDir)
    initEditor(name, launcherDir)

    trymkdir(gameDir)
    trymkdir(editorDir)

end

initLauncher("launcher")

