function setLinks(someLinks)
    links = someLinks
end

include "../premake/utils.lua"

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
    local binDir = os.realpath(solutionDir .. "../bin/build")

    local outputDir = os.realpath(binDir .. "/debug")

    local debugDir = os.realpath(solutionDir .. "../bin/resource/")

    local tempDir = os.realpath(solutionDir .. "../temp/%{cfg.buildcfg}")

    trymkdir(outputDir)
    trymkdir(tempDir)

    -- targetdir(outputDir) -- ouput dir  
    -- if not (os.isdir(outputDir)) then
    --     os.mkdir(os.realpath(outputDir))
    -- end

    -- objdir(tempDir) -- intermediate dir
    -- if not (os.isdir(tempDir)) then
    --     os.mkdir(os.realpath(tempDir))
    -- end

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    debugdir(debugDir)
    targetdir(outputDir)
    objdir(tempDir)

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
        "src/",
        "../vendor/src/"
    }

    files {
        "src/**.h",
        "src/**.hpp",
        "src/**.c",
        "src/**.cpp"
    }

    outputDir = os.realpath(outputDir .. "/")

    local releaseDir = os.realpath(binDir .. "/release")

    trymkdir(releaseDir)

    local excludeArgs = "editor_Debug.*\neditor_Release.*"
    local exclude = "../premake/exclude.txt"
    io.writefile(exclude, excludeArgs)

    local releaseCpy = "xcopy " .. outputDir .. " " .. releaseDir .. " /y " .. "/u " .. "/f " .. "/exclude:" ..
                           os.realpath(exclude) .. " "

 

    postbuildcommands {
        releaseCpy
    }
    -- local gameToEditorDirCpy = "xcopy " .. gameDir .. " " .. editorDir .. " /y "

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

    initEngine(name, outputDir, debugDir, releaseCpy, tempDir)
    initEditor(name, outputDir, debugDir, tempDir)

end

initLauncher("launcher")

