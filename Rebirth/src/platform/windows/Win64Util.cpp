// ------------------------------------------------------------------------------
// 
// Rebirth
//    Copyright 2022 Matthew Rogers
// 
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
// 
// File Name: Win64Util.cpp
// Date File Created: 6/30/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "rebirth/util/PlatformUtil.h"
#include "rebirth/core/Application.h"

#include <commdlg.h>
#include <glfw/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/glfw3native.h>

namespace rebirth
{
	std::string FileDialog::OpenFile(const char* filters)
	{
		OPENFILENAMEA openFileName;
		CHAR fileSize[256] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Instance().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = fileSize;
		openFileName.nMaxFile = sizeof(fileSize);
		if (GetCurrentDirectoryA(256, currentDir))
		{
			openFileName.lpstrInitialDir = currentDir;
		}
		openFileName.lpstrFilter = filters;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filters)
	{
		OPENFILENAMEA openFileName;
		CHAR fileSize[256] = { 0 };
		CHAR currentDir[256] = { 0 };
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Instance().GetWindow().GetNativeWindow());
		openFileName.lpstrFile = fileSize;
		openFileName.nMaxFile = sizeof(fileSize);
		if (GetCurrentDirectoryA(256, currentDir))
		{
			openFileName.lpstrInitialDir = currentDir;
		}
		openFileName.lpstrFilter = filters;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

		openFileName.lpstrDefExt = strchr(filters, '\0') + 1;

		if (GetSaveFileNameA(&openFileName) == TRUE)
		{
			return openFileName.lpstrFile;
		}

		return std::string();
	}

	struct TimerData
	{
		uint64 frequency = 0;
		uint64 offset = 0;
	};

	static TimerData sData;

	void Time::Init()
	{
		QueryPerformanceFrequency((LARGE_INTEGER*) &sData.frequency);
		sData.offset = GetTimerValue();
	}

	uint64 Time::GetTimerValue()
	{
		uint64 ret;
		QueryPerformanceCounter((LARGE_INTEGER*)&ret);
		return ret;
	}

	uint64 Time::GetTimerFrequency()
	{
		return sData.frequency;
	}

	double Time::GetTime()
	{
		return (double)(GetTimerValue() - sData.offset) / GetTimerFrequency();
	}
}