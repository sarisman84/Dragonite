-- function copyDir(aSource, aDestination)
--     os.execute("xcopy /S /Y " .. aSource .. " " .. aDestination)
--     return aDestination
-- end
-- function fetchVendorInclude(aVendorName, aWorkingDir)
--     local ogWorkingDir = aWorkingDir .. "/vendor/"
--     local finalVendorDir
--     aWorkingDir = os.realpath(aWorkingDir .. "/vendor/" .. aVendorName)
--     local vendor = os.realpath("../vendor/" .. aVendorName)
--     printf("Working Dir: " .. aWorkingDir)
--     local r = os.isdir(aWorkingDir)
--     if (r) then
--         printf("Work Dir is valid")
--     else
--         printf("Work Dir is not valid")
--         os.mkdir(aWorkingDir)
--     end
--     printf("Vendor: " .. vendor)
--     r = os.isdir(vendor);
--     if (r) then
--         printf("Found Vendor")
--         finalVendorDir = copyDir(vendor, aWorkingDir)
--     else
--         printf("Could not find vendor")
--     end
--     return finalVendorDir
-- end
function trymkdir(aDir)
    if not (os.isdir(aDir)) then
        os.mkdir(os.realpath(aDir))
    end
end

local src = "src/"
local cOutput = "../lib/"
local outputTemp = "../temp/"

project "vendor"
location "."
kind "StaticLib"
language "C++"
cppdialect "C++20"

output = "%{cfg.buildcfg}"
solutionDir = (src)
prjName = "%{prj.name}"

targetdir(cOutput)
objdir(outputTemp)
debugdir(cOutput)

trymkdir(cOutput)
trymkdir(outputTemp)

targetname("%{prj.name}_%{cfg.buildcfg}")

files {src .. "**.h", src .. "**.hpp", src .. "**.c", src .. "**.cpp",
       src .. "../util/vcpkg/packages/freetype_**/include/"}

removefiles {src .. "nlohmann/include/**", src .. "nlohmann/tests/**", src .. "nlohmann/tools/**",
             src .. "nlohmann/docs/**", src .. "imguizmo/example/**", src .. "imguizmo/vcpkg-example/**",
             src .. "imgui/examples/**", src .. "imgui/backends/**", src .. "entt/src/**", src .. "entt/test/**",
             src .. "entt/conan/**"}
files {src .. "imgui/backends/imgui_impl_dx11.**", src .. "imgui/backends/imgui_impl_win32.**"}

excludes {"ffmpeg-2.0/**.h", "ffmpeg-2.0/**.c", "ffmpeg-2.0/**.cpp", "DirectXTex/DirectXTex/**",
          "DirectXTex/DDSView/**", "DirectXTex/Texassemble/**", "DirectXTex/Texconv/**", "DirectXTex/Texdiag/**",
          "DirectXTex/DDSTextureLoader/DDSTextureLoader9.**", "DirectXTex/DDSTextureLoader/DDSTextureLoader12.**",
          "DirectXTex/ScreenGrab/ScreenGrab9.**", "DirectXTex/ScreenGrab/ScreenGrab12.**",
          "DirectXTex/WICTextureLoader/WICTextureLoader9.**", "DirectXTex/WICTextureLoader/WICTextureLoader12.**"}
includedirs {".", src, src .. "nlohmann/", src .. "imgui/", src .. "imguizmo/", src .. "entt/",
             src .. "entt/single_include/"}
libdirs {cOutput, src .. "../util/vcpkg/packages/freetype_**/lib/"}
defines {"_CONSOLE"}

filter "configurations:Debug"
defines {"_DEBUG"}
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

filter "system:windows"
kind "StaticLib"
staticruntime "off"
symbols "On"
systemversion "latest"
-- warnings "Extra"
-- conformanceMode "On"
-- buildoptions { "/permissive" }
flags { --	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
--	"FatalCompileWarnings",
"MultiProcessorCompile"}
links {"DXGI", "dxguid"}
defines {"_WIN32_WINNT=0x0601"}

