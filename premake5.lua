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
IncludeDir["spdlog"] = "vendor/deps/spdlog/include"
IncludeDir["GLFW"] = "vendor/deps/glfw/include"
IncludeDir["Glad"] = "vendor/deps/Glad/include"
IncludeDir["ImGui"] = "vendor/deps/imgui"
IncludeDir["glm"] = "vendor/deps/glm"
IncludeDir["stb_image"] = "vendor/deps/stb_image"
IncludeDir["entt"] = "vendor/deps/entt/include"
IncludeDir["yaml_cpp"] = "vendor/deps/yaml-cpp/include"

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