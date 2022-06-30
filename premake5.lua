workspace "Rebirth"
	architecture "x86_64"
	startproject "Rebirth-Reedit"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "Rebirth/libs/spdlog/include"
IncludeDir["GLFW"] = "Rebirth/libs/glfw/include"
IncludeDir["Glad"] = "Rebirth/libs/Glad/include"
IncludeDir["ImGui"] = "Rebirth/libs/imgui"
IncludeDir["glm"] = "Rebirth/libs/glm"
IncludeDir["stb_image"] = "Rebirth/libs/stb_image"
IncludeDir["entt"] = "Rebirth/libs/entt/include"
IncludeDir["yaml_cpp"] = "Rebirth/libs/yaml-cpp/include"

-- Includes the premake file added to the glfw fork/submodule
group "Dependencies"
	include "Rebirth/libs/glfw"
	include "Rebirth/libs/Glad"
	include "Rebirth/libs/imgui"
	include "Rebirth/libs/yaml-cpp"
group ""

project "Rebirth"
	location "Rebirth"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	pchheader "rbpch.h"
	pchsource "Rebirth/src/rbpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/libs/stb_image/stb_image.cpp",
		"premake5.lua"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"

--		defines
--		{
--			"RB_PLATFORM_WINDOWS"
--		}

--		postbuildcommands
--		{
--			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
--		}

	filter "configurations:Debug"
		defines
		{
			"RB_DEBUG"
		}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"

--	filters { "system:windows", "configurations:Release" }
--		buildoptions "/MT"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rebirth/src",
		"Rebirth/libs",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
	}

	links
	{
		"Rebirth"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines
		{
			"RB_DEBUG"
		}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"





project "Rebirth-Reedit"
	location "Rebirth-Reedit"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Rebirth/src",
		"Rebirth/libs",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
	}

	links
	{
		"Rebirth"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines
		{
			"RB_DEBUG"
		}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"