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
// File Name: Win64Window.cpp
// Date File Created: 06/15/2022 at 10:54 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "Win64Window.h"

namespace rebirth
{
	static bool gGlfwInitialized = false;

	Window* Window::Create(const WindowProperties& props)
	{
		return new Win64Window(props);
	}

}


rebirth::Win64Window::Win64Window(const WindowProperties& props)
{
	Init(props);
}

rebirth::Win64Window::~Win64Window()
{
	Shutdown();
}

void rebirth::Win64Window::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(mWindow);
}

void rebirth::Win64Window::SetVSync(bool enabled)
{
	if(enabled)
	{
		glfwSwapInterval(1);
	}else
	{
		glfwSwapInterval(0);
	}

	mData.vSync = enabled;
}
bool rebirth::Win64Window::IsVSync() const
{
	return mData.vSync;
}

void rebirth::Win64Window::Init(const WindowProperties& props)
{
	mData.title = props.title;
	mData.width = props.width;
	mData.height = props.height;

	RB_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

	if(!gGlfwInitialized)
	{
		int success = glfwInit();
		RB_CORE_ASSERT(success, "Could not initialize GLFW")

		gGlfwInitialized = true;
	}

	mWindow = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), mData.title.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(mWindow);
	glfwSetWindowUserPointer(mWindow, &mData);
	SetVSync(true);
}

void rebirth::Win64Window::Shutdown()
{
	glfwDestroyWindow(mWindow);
}
