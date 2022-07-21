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


namespace rebirth
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(uint32 width, uint32 height) : Event(EventType::WINDOW_RESIZE), mWidth(width), mHeight(height) {}

		uint32 GetWidth() const { return mWidth; }
		uint32 GetHeight() const { return mHeight; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << mWidth << ", " << mHeight;
			return ss.str();
		}

		//EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)

	private:
		uint32 mWidth;
		uint32 mHeight;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() : Event(EventType::WINDOW_CLOSE) {}

		std::string ToString() const override { return "WindowCloseEvent"; }
		//EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	};

	//class TickEvent : public Event
	//{
	//public:
	//	TickEvent() = default;

	//	EVENT_CLASS_TYPE(TICK)
	//	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	//};

	//class UpdateEvent : public Event
	//{
	//public:
	//	UpdateEvent() = default;

	//	EVENT_CLASS_TYPE(UPDATE)
	//	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	//};

	//class RenderEvent : public Event
	//{
	//public:
	//	RenderEvent() = default;

	//	EVENT_CLASS_TYPE(RENDER)
	//	EVENT_CLASS_CATEGORY(EVENT_CATEGORY_APP)
	//};
}
