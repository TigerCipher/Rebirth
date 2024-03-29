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

#include "rebirth/core/Window.h"

#include <GLFW/glfw3.h>
#include "rebirth/renderer/GraphicsContext.h"


namespace rebirth
{
	class Win64Window : public Window
	{
	public:
		Win64Window(const ApplicationDesc& props);

		virtual ~Win64Window();

		void OnUpdate() override;

		uint32 GetWidth() const override { return mData.width; }
		uint32 GetHeight() const override { return mData.height; }

		void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		float GetHighDPIScaleFactor() const override { return mHighDpiScaleFactor; }

		void* GetNativeWindow() const override { return mWindow; }
	private:
		GLFWwindow* mWindow;
		Scope<GraphicsContext> mContext;

		struct WindowData
		{
			std::string title;
			uint32 width, height;
			bool vSync;
			EventCallbackFn eventCallback;
		};

		WindowData mData;
		float mHighDpiScaleFactor = 1.0f;

		virtual void Init(const ApplicationDesc& props);
		virtual void Shutdown();
	};
}