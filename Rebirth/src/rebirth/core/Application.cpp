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
#include "rebirth/util/PlatformUtil.h"
#include "rebirth/debug/Statistics.h"
#include "rebirth/imgui/Panels.h"

// temp
#include <glfw/glfw3.h>

namespace rebirth
{
	Application* Application::sInstance = nullptr;

	Application::Application(ApplicationDesc appDesc, CommandLineArgs cmd) :
		mCommandLine(cmd),
		mDispatcher("Event System")
	{
		RB_PROFILE_FUNC();
		RB_CORE_ASSERT(!sInstance, "Application already exists");
		sInstance = this;
		Panels::Init();
		RB_CORE_INFO("Creating core application");
		Time::Init();
		mWindow = Window::Create(appDesc);
		mWindow->SetEventCallback(std::bind(&Application::HandleEvents, this, std::placeholders::_1));
		Panels::PostInit();


		Renderer::Init();


		mImguiLayer = new ImguiLayer();
		PushOverlay(mImguiLayer);

		mDispatcher.AddListener(this);
		RB_CORE_TRACE("Core application created");
	}

	Application::~Application()
	{
		RB_PROFILE_FUNC();
		RB_CORE_INFO("Shutting down application");
		Renderer::Shutdown();
	}

	void Application::Close()
	{
		RB_CORE_INFO("Closing application and exiting the run loop");
		mRunning = false;
	}

	void Application::Run()
	{
		RB_PROFILE_FUNC();
		float accumulator = 0;
		int fps = 0;
		while (mRunning)
		{
			RB_PROFILE_SCOPE("Run Loop");
			auto time = (float)Time::GetTime();
			Timestep timestep = time - mLastFrameTime;
			Statistics::SetFrameTime(timestep);
			accumulator += timestep;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				RB_PROFILE_SCOPE("Update LayerStack");
				for (Layer* layer : mLayerStack)
				{
					layer->OnUpdate(timestep);
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
			}

			++fps;
			mWindow->OnUpdate();
			mDispatcher.PollEvents();

			if (accumulator >= 1.0f)
			{
				Statistics::SetFPS(fps);
				accumulator = 0;
				fps = 0;
			}
		}
	}

	void Application::HandleEvents(Event* e)
	{
		RB_PROFILE_FUNC();
		mDispatcher.Dispatch(e);

		//for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it)
		//{
		//	//if (e.mHandled)
		//	//	break;
		//	(*it)->OnEvent(e);
		//}
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

	void Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
	}

	void Application::OnWindowResize(WindowResizeEvent& e)
	{
		RB_PROFILE_FUNC();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return;
		}
		mMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

	}

}