

function initEditor(aName, aDir)
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

    local tDir = aDir .. "/editor"
    local tempDir = tDir .. "/temp"
    targetdir(tDir) -- ouput dir  
    objdir(tempDir) -- intermediate dir
    debugdir(tDir)
    if not (os.isdir(tDir)) then
        os.mkdir(os.realpath(tDir))
    end

    if not (os.isdir(tempDir)) then
        os.mkdir(os.realpath(tempDir))
    end

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    includedirs {src, src .. "/shaders/includes", "../vendor/src/"}
    libdirs {"../lib"}

    files {src .. "**.h", src .. "**.hpp", src .. "**.c", src .. "**.cpp", src .. "shaders/**.hlsl",
           src .. "shaders/includes/**.hlsli"}
    return tDir
end
