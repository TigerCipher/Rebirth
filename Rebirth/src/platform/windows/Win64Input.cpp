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
// File Name: Win64Input.h
// Date File Created: 06/18/2022 at 4:09 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#include "rbpch.h"


#include "Win64Input.h"
#include "rebirth/Application.h"

#include <GLFW/glfw3.h>

rebirth::Input* rebirth::Input::sInstance = new Win64Input();

bool rebirth::Win64Input::IsKeyPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool rebirth::Win64Input::IsMouseButtonPressedImpl(int button)
{
	auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());

	auto state = glfwGetMouseButton(window, button);

	return state == GLFW_PRESS;
}

float rebirth::Win64Input::GetMouseXImpl()
{
	auto[x, y] = GetMousePosImpl();
	return x;
}

float rebirth::Win64Input::GetMouseYImpl()
{
	auto [x, y] = GetMousePosImpl();
	return y;
}

std::pair<float, float> rebirth::Win64Input::GetMousePosImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return { static_cast<float>(x), static_cast<float>(y) };
}
