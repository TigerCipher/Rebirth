project "Sandbox"
	--location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
        "src",
		"%{wks.location}/Rebirth/src",
		"%{wks.location}/vendor/deps/",
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
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} \"%{IncludeDir.zlib}/bin/Debug-windows-x86_64/zlib/zlib.dll\" \"%{cfg.targetdir}\"",
		}

	filter "configurations:Release"
		defines "RB_RELEASE"
		optimize "on"
		runtime "Release"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} \"%{IncludeDir.zlib}/bin/Release-windows-x86_64/zlib/zlib.dll\" \"%{cfg.targetdir}\"",
		}

	filter "configurations:Dist"
		defines "RB_DIST"
		runtime "Release"
		optimize "on"