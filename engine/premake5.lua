include "vendor"
function initEngine(aName, aDir, aWorkingDir, aPostBuildEventCommand, aTempDir)
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

    targetdir(aDir) -- ouput dir  
    objdir(aTempDir) -- intermediate dir
    debugdir(aWorkingDir)

    -- trymkdir(aDir)
    -- trymkdir(aWorkingDir)

    targetname("%{prj.name}_%{cfg.buildcfg}") -- target name

    includedirs {src, src .. "/shaders/includes", "../vendor/src/"}

    libdirs {"../lib"}

    links {"vendor"}

    files {src .. "**.h", src .. "**.hpp", src .. "**.c", src .. "**.cpp", src .. "**.hlsl", src .. "**.hlsli"}

    postbuildcommands {aPostBuildEventCommand}

    shadermodel("5.0")
	local shader_dir = src
	-- os.mkdir(shader_dir)

    local shaderOutput = aWorkingDir.."shaders/"

    trymkdir(shaderOutput)


	filter("files:**.hlsl")
		flags("ExcludeFromBuild")
		shaderobjectfileoutput(shaderOutput .. "%{file.basename}"..".cso")

	filter("files:**PS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Pixel")

	filter("files:**VS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Vertex")

	filter("files:**GS.hlsl")
		removeflags("ExcludeFromBuild")
		shadertype("Geometry")

	-- Warnings as errors
	shaderoptions({"/WX"})





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

