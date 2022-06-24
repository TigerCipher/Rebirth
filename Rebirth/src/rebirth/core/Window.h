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
// File Name: Window.h
// Date File Created: 06/15/2022 at 10:39 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "rbpch.h"

#include "rebirth/core/Common.h"
#include "rebirth/events/Event.h"

namespace rebirth
{
	struct WindowProperties
	{
		std::string title;
		uint width;
		uint height;

		WindowProperties(const std::string& pTitle = "Rebirth Engine", uint pWidth = 1920, uint pHeight = 1080) : title(pTitle), width(pWidth), height(pHeight) {}
	};

	class RB_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual uint GetWidth() const = 0;
		virtual uint GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());
	};
}