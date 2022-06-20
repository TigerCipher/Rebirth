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

#include "rebirth/events/AppEvent.h"
#include "rebirth/events/KeyEvent.h"
#include "rebirth/events/MouseEvent.h"

#include "platform/opengl/OpenGLContext.h"

namespace rebirth
{
	static bool gGlfwInitialized = false;

	Window* Window::Create(const WindowProperties& props)
	{
		return new Win64Window(props);
	}

	static void GlfwErrorCallback(int error, const char* desc)
	{
		RB_CORE_ERROR("GLFW Error ({0}) -> {1}", error, desc);
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
	mContext->SwapBuffers();
}

void rebirth::Win64Window::SetVSync(bool enabled)
{
	if (enabled)
	{
		glfwSwapInterval(1);
	}
	else
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
	mData.title  = props.title;
	mData.width  = props.width;
	mData.height = props.height;


	RB_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

	
	if (!gGlfwInitialized)
	{
		RB_CORE_TRACE("Initializing GLFW");
		const int success = glfwInit();
		RB_CORE_ASSERT(success, "Could not initialize GLFW")
		RB_CORE_TRACE("GLFW Initialized");

		glfwSetErrorCallback(GlfwErrorCallback);

		gGlfwInitialized = true;
	}

	RB_CORE_TRACE("Creating glfw window");
	mWindow = glfwCreateWindow(static_cast<int>(props.width), static_cast<int>(props.height), mData.title.c_str(),
	                           nullptr, nullptr);

	const int maxWidth  = GetSystemMetrics(SM_CXSCREEN);
	const int maxHeight = GetSystemMetrics(SM_CYSCREEN);
	glfwSetWindowMonitor(mWindow, nullptr, (maxWidth / 2) - (props.width / 2), (maxHeight / 2) - (props.height / 2), props.width, props.height, GLFW_DONT_CARE);

	mContext = new OpenGLContext(mWindow);
	mContext->Init();
	
	
	RB_CORE_TRACE("Setting glfw window pointer");
	glfwSetWindowUserPointer(mWindow, &mData);
	RB_CORE_TRACE("TEST");
	SetVSync(true);


	glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		data.width       = width;
		data.height      = height;

		WindowResizeEvent event(width, height);
		data.eventCallback(event);
	});

	glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event;
		data.eventCallback(event);
	});

	glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.eventCallback(event);
				break;
			}
		case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.eventCallback(event);
				break;
			}
		case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.eventCallback(event);
				break;
			}
		}

		RB_CORE_TRACE("TEST2");
	});

	glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.eventCallback(event);
				break;
			}
		case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.eventCallback(event);
				break;
			}
		}
	});

	glfwSetScrollCallback(mWindow, [](GLFWwindow* window, const double xOffset, const double yOffset)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
		data.eventCallback(event);
	});

	glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
	{
		WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

		MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
		data.eventCallback(event);
	});
}

void rebirth::Win64Window::Shutdown()
{
	glfwDestroyWindow(mWindow);
}
