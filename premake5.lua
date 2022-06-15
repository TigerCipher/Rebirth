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

project "Rebirth"
	location "Rebirth"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/libs/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RB_WINDOWS",
			"RB_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "RB_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RB_DIST"
		optimize "On"

--	filters { "system:windows", "configurations:Release" }
--		buildoptions "/MT"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"Rebirth/src"
	}

	links
	{
		"Rebirth"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"RB_WINDOWS"
		}


	filter "configurations:Debug"
		defines "RB_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RB_DIST"
		optimize "On"