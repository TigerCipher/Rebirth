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


#include <glad/glad.h>

#include "platform/windows/Win64Window.h"
#include "Input.h"

rebirth::Application* rebirth::Application::sInstance = nullptr;

rebirth::Application::Application()
{
	RB_CORE_ASSERT(!sInstance, "Application already exists");
	sInstance = this;
	//mWindow = createScope<Win64Window>(Window::Create());
	mWindow = std::unique_ptr<Window>(Window::Create());
	mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
}
rebirth::Application::~Application() {}

void rebirth::Application::Run()
{
	
	while(mRunning)
	{
		glClearColor(0.2f, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);


		for(Layer* layer : mLayerStack)
		{
			layer->OnUpdate();
		}
		
		mWindow->OnUpdate();
	}
}

void rebirth::Application::OnEvent(Event& e)
{
	EventDispatcher disp(e);
	disp.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

	for(auto it = mLayerStack.end(); it != mLayerStack.begin();)
	{
		(*--it)->OnEvent(e);
		if(e.WasHandled())
		{
			break;
		}
	}
}

void rebirth::Application::PushLayer(Layer* layer)
{
	mLayerStack.PushLayer(layer);
	layer->OnAttach();
}

void rebirth::Application::PushOverlay(Layer* overlay)
{
	mLayerStack.PushOverlay(overlay);
	overlay->OnAttach();
}

bool rebirth::Application::OnWindowClose(WindowCloseEvent& e)
{
	mRunning = false;
	return true;
}
