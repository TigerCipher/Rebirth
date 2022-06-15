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
// File Name: AppEvent.h
// Date File Created: 06/14/2022 at 11:29 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


#include "Event.h"

#include <sstream>

namespace rebirth
{
	class RB_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint width, uint height) : mWidth(width), mHeight(height) {}

		[[nodiscard]] uint GetWidth() const { return mWidth; }
		[[nodiscard]] uint GetHeight() const { return mHeight; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)

	private:
		uint mWidth;
		uint mHeight;
	};

	class RB_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	};

	class RB_API TickEvent : public Event
	{
	public:
		TickEvent() = default;

		EVENT_CLASS_TYPE(TICK)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	};

	class RB_API UpdateEvent : public Event
	{
	public:
		UpdateEvent() = default;

		EVENT_CLASS_TYPE(UPDATE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	};

	class RB_API RenderEvent : public Event
	{
	public:
		RenderEvent() = default;

		EVENT_CLASS_TYPE(RENDER)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	};
}
