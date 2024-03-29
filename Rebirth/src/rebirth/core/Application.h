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
// File Name: Application.h
// Date File Created: 06/13/2022 at 3:04 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "rebirth/core/Common.h"
#include "Window.h"
#include "LayerStack.h"
#include "rebirth/events/Event.h"
#include "rebirth/events/AppEvent.h"
#include "rebirth/events/WindowEventListener.h"
#include "rebirth/events/EventDispatcher.h"

#include "rebirth/imgui/ImguiLayer.h"

#include "rebirth/renderer/Shader.h"
#include "rebirth/renderer/Buffer.h"
#include "rebirth/renderer/VertexArray.h"

#include "Timestep.h"
#include "ApplicationDesc.h"

namespace rebirth
{

	struct CommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			RB_CORE_ASSERT(index < count);
			return args[index];
		}
	};

	class Application : public WindowEventListener
	{
	public:
		Application(ApplicationDesc appDesc, CommandLineArgs cmd = CommandLineArgs());
		virtual ~Application();

		void Run();
		void Close();

		void HandleEvents(Event* e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		void OnWindowClose(WindowCloseEvent& e) override;
		void OnWindowResize(WindowResizeEvent& e) override;

		Window& GetWindow() const { return *mWindow; }
		ImguiLayer* GetImguiLayer() { return mImguiLayer; }

		CommandLineArgs GetCommandLineArgs() const { return mCommandLine; }

		EventDispatcher& GetEventDispatcher() { return mDispatcher; }

		static Application& Instance() { return *sInstance; }

	private:

		static Application* sInstance;
		CommandLineArgs mCommandLine;
		Scope<Window> mWindow;

		EventDispatcher mDispatcher;

		LayerStack mLayerStack;
		ImguiLayer* mImguiLayer;
		float mLastFrameTime = 0.0f;

		bool mRunning = true;
		bool mMinimized = false;

	};

	// Defined by game
	Application* CreateApplication(CommandLineArgs args);
}
