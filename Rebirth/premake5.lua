project "Rebirth"
	--location "Rebirth"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "rbpch.h"
	pchsource "src/rbpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{wks.location}/vendor/deps/stb_image/stb_image.cpp",
		"%{wks.location}/vendor/deps/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.ImGuizmo}",
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
	filter "files:../vendor/deps/ImGuizmo/**.cpp"
		flags {"NoPCH"}

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