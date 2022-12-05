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
    local binDir = os.realpath(solutionDir .. "../bin/build")
    local launcherDir = os.realpath(binDir .. "/launcher")
    local outputDir = launcherDir
    local debugDir = os.realpath(solutionDir .. "../bin/resc")
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

    flags {"MultiProcessorCompile"}

    links {"engine", "editor"}

    libdirs {"../lib"}

    includedirs {"src/", "../vendor/src/"}

    files {"src/**.h", "src/**.hpp", "src/**.c", "src/**.cpp"}

    outputDir = os.realpath(outputDir .. "/")

    local gameDir = os.realpath(binDir .. "/engine")
    local editorDir = os.realpath(binDir .. "/editor")

    local gameDirCpy = "xcopy " .. outputDir .. " " .. gameDir .. " /y "
    local editorDirCpy = "xcopy " .. outputDir .. " " .. editorDir .. " /y "
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

    initEngine(name, gameDir, debugDir, gameDirCpy)
    initEditor(name, editorDir, debugDir, editorDirCpy)

    trymkdir(gameDir)
    trymkdir(editorDir)

end

initLauncher("launcher")

