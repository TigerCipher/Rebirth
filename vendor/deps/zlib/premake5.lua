project "zlib"
kind "StaticLib"
language "C"
staticruntime "on"
systemversion "latest"

targetdir ("bin/" .. outputdir .. "/%{prj.name}")
objdir ("obj/" .. outputdir .. "/%{prj.name}")

files
{
    "*.c",
    "*.h"
}

filter "configurations:Release"
    runtime "Release"
    optimize "on"

filter "configurations:Dist"
    runtime "Release"
    optimize "on"
    symbols "Off"

filter "configurations:Debug"
    runtime "Debug"
    symbols "on"