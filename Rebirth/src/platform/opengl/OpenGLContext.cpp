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
}

void rebirth::OpenGLContext::SwapBuffers()
{
	//TODO TEMP: testing some basic immediate mode drawing
	// Tested using compatibility Glad - now switched to Core so this is obsolete
	//glColor3f(0.0f, 1.0f, 0.4f);
	//glBegin(GL_TRIANGLES);//start drawing triangles
	//glVertex3f(-1.0f, -0.1f, 0.0f);//triangle one first vertex
	//glVertex3f(-0.5f, -0.25f, 0.0f);//triangle one second vertex
	//glVertex3f(-0.75f, 0.25f, 0.0f);//triangle one third vertex
	////drawing a new triangle
	//glVertex3f(0.5f, -0.25f, 0.0f);//triangle two first vertex
	//glVertex3f(1.0f, -0.25f, 0.0f);//triangle two second vertex
	//glVertex3f(0.75f, 0.25f, 0.0f);//triangle two third vertex
	//glEnd();//end drawing of triangles
	glfwSwapBuffers(mWindow);
}
