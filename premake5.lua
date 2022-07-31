include "vendor/premake/solution_items.lua"
include "Dependencies.lua"

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

group "Utilities"
	include "vendor/premake"
	include "scripts"
group ""

group "Dependencies"
	include "vendor/deps/glfw"
	include "vendor/deps/Glad"
	include "vendor/deps/imgui"
	include "vendor/deps/yaml-cpp"
	include "vendor/deps/box2d"
	include "vendor/deps/zlib"
group ""

include "Rebirth"
include "Rebirth-Reedit"
include "Sandbox"