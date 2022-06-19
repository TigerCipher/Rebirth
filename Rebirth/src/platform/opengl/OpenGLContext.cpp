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
// File Name: OpenGLContext.cpp
// Date File Created: 06/19/2022 at 1:01 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"


#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include <glad/glad.h>

rebirth::OpenGLContext::OpenGLContext(GLFWwindow* window): mWindow(window)
{
	RB_CORE_ASSERT(window, "Window handle is nullptr");
}

void rebirth::OpenGLContext::Init()
{
	RB_CORE_TRACE("Creating OpenGL context");
	glfwMakeContextCurrent(mWindow);

	RB_CORE_TRACE("Loading Glad (OpenGL)");
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	RB_CORE_ASSERT(status, "Failed to initialize Glad");
	RB_CORE_TRACE("Glad (OpenGL) loaded!");

	const char* rendererName = (char*)glGetString(GL_RENDERER);
	const char* vendorName = (char*)glGetString(GL_VENDOR);
	const char* glVersionName = (char*)glGetString(GL_VERSION);
	RB_CORE_INFO("Detected graphics unit: {} ({})", rendererName, vendorName);
	RB_CORE_INFO("Available OpenGL Version: {}", glVersionName);

	RB_CORE_TRACE("OpenGL Context created");
}

void rebirth::OpenGLContext::SwapBuffers()
{
	glfwSwapBuffers(mWindow);
}
