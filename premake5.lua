include "vendor/premake/solution_items.lua"

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

	solution_items
	{
		".editorconfig",
		".gitignore",
		".gitmodules",
		"README.md"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "%{wks.location}/vendor/deps/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/vendor/deps/glfw/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/deps/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/vendor/deps/imgui"
IncludeDir["glm"] = "%{wks.location}/vendor/deps/glm"
IncludeDir["stb_image"] = "%{wks.location}/vendor/deps/stb_image"
IncludeDir["entt"] = "%{wks.location}/vendor/deps/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/vendor/deps/yaml-cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "vendor/deps/glfw"
	include "vendor/deps/Glad"
	include "vendor/deps/imgui"
	include "vendor/deps/yaml-cpp"
group ""

include "Rebirth"
include "Rebirth-Reedit"
include "Sandbox"