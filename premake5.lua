workspace "Rebirth"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Rebirth/libs/glfw/include"
IncludeDir["Glad"] = "Rebirth/libs/Glad/include"
IncludeDir["ImGui"] = "Rebirth/libs/imgui"
IncludeDir["glm"] = "Rebirth/libs/glm"

-- Includes the premake file added to the glfw fork/submodule
group "Dependencies"
	include "Rebirth/libs/glfw"
	include "Rebirth/libs/Glad"
	include "Rebirth/libs/imgui"

group ""

project "Rebirth"
	location "Rebirth"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	pchheader "rbpch.h"
	pchsource "Rebirth/src/rbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"premake5.lua"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/libs/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RB_WINDOWS",
			"RB_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
		}

	filter "configurations:Debug"
		defines
		{
			"RB_DEBUG"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "On"

--	filters { "system:windows", "configurations:Release" }
--		buildoptions "/MT"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rebirth/libs/spdlog/include",
		"Rebirth/src",
		"%{IncludeDir.glm}",
		"Rebirth/libs"
	}

	links
	{
		"Rebirth"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"RB_WINDOWS"
		}


	filter "configurations:Debug"
		defines
		{
			"RB_DEBUG",
			"RB_ENABLE_ASSERTS"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "On"