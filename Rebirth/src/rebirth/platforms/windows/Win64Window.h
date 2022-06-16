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
// File Name: Win64Window.h
// Date File Created: 06/15/2022 at 10:50 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "rebirth/Window.h"

#include <GLFW/glfw3.h>

namespace rebirth
{
	class Win64Window : public Window
	{
	public:
		Win64Window(const WindowProperties& props);

		virtual ~Win64Window();

		void OnUpdate() override;

		uint GetWidth() const override { return mData.width; }
		uint GetHeight() const override { return mData.height; }

		void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }

		void SetVSync(bool enabled) override;

		bool IsVSync() const override;

	
	private:
		GLFWwindow* mWindow;

		struct WindowData
		{
			std::string title;
			uint width, height;
			bool vSync;
			EventCallbackFn eventCallback;
		};

		WindowData mData;

		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();
	};
}