project "zlib"
kind "SharedLib"
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

filter "system:windows"
    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
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