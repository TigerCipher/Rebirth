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

#include "rebirth/renderer/Renderer.h"

// temp
#include <glfw/glfw3.h>

namespace rebirth
{
	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		RB_CORE_ASSERT(!sInstance, "Application already exists");
		RB_CORE_TRACE("Creating core application");
		sInstance = this;
		mWindow = Scope<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		//mWindow->SetVSync(false);


		Renderer::Init();


		mImguiLayer = new ImguiLayer();
		PushOverlay(mImguiLayer);

		RB_CORE_TRACE("Core application created");
	}

	void Application::Run()
	{
		while (mRunning)
		{

			// TODO: Abstract getTime to platform/opengl
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - mLastFrameTime;
			mLastFrameTime = time;

			for (Layer* layer : mLayerStack)
			{
				layer->OnUpdate(timestep);
			}

			mImguiLayer->Begin();
			for (Layer* layer : mLayerStack)
			{
				layer->OnImguiRender();
			}
			mImguiLayer->End();

			mWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		disp.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.WasHandled())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		mLayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

}