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
		RB_PROFILE_FUNC();
		RB_CORE_ASSERT(!sInstance, "Application already exists");
		RB_CORE_TRACE("Creating core application");
		sInstance = this;
		mWindow = Scope<Window>(Window::Create());
		mWindow->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		//mWindow->SetVSync(false); //#NOTE: Should maybe leave on by default


		Renderer::Init();


		mImguiLayer = new ImguiLayer();
		PushOverlay(mImguiLayer);

		RB_CORE_TRACE("Core application created");
	}

	Application::~Application()
	{
		RB_PROFILE_FUNC();
		Renderer::Shutdown();
	}

	void Application::Close()
	{
		mRunning = false;
	}

	void Application::Run()
	{
		RB_PROFILE_FUNC();
		while (mRunning)
		{
			RB_PROFILE_SCOPE("Run Loop");
			// #TODO: Abstract getTime to platform/opengl
			float time = static_cast<float>(glfwGetTime());
			Timestep timestep = time - mLastFrameTime;
			mLastFrameTime = time;

			if(!mMinimized)
			{
				RB_PROFILE_SCOPE("Update LayerStack");
				for (Layer* layer : mLayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			mImguiLayer->Begin();
			{
				RB_PROFILE_SCOPE("LayerStack render imgui");
				for (Layer* layer : mLayerStack)
				{
					layer->OnImguiRender();
				}
			}
			mImguiLayer->End();

			mWindow->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		RB_PROFILE_FUNC();
		EventDispatcher disp(e);
		disp.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(Application::OnWindowClose));
		disp.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(Application::OnWindowResize));

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
		RB_PROFILE_FUNC();
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		RB_PROFILE_FUNC();
		mLayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		RB_PROFILE_FUNC();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return false;
		}
		mMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}