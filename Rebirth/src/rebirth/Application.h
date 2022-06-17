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

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "events/Event.h"
#include "events/AppEvent.h"

namespace rebirth
{
	class RB_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

	private:
		UniquePtr<Window> mWindow;
		bool mRunning = true;
		LayerStack mLayerStack;

		bool OnWindowClose(WindowCloseEvent& e);
	};

	// Defined by game
	Application* CreateApplication();
}