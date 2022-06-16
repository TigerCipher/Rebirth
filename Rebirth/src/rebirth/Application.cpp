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
// File Name: Application.cpp
// Date File Created: 06/13/2022 at 3:04 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"

#include "Application.h"

#include <GLFW/glfw3.h>

#include "platforms/windows/Win64Window.h"

rebirth::Application::Application()
{
	//mWindow = createScope<Win64Window>(Window::Create());
	mWindow = std::unique_ptr<Window>(Window::Create());
}
rebirth::Application::~Application() {}

void rebirth::Application::Run()
{
	
	while(mRunning)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		mWindow->OnUpdate();
	}
}
