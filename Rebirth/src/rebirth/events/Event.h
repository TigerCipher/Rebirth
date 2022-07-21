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
// File Name: Event.h
// Date File Created: 06/14/2022 at 11:14 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

namespace rebirth
{
	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE,
		WINDOW_RESIZE,
		WINDOW_FOCUS,
		WINDOW_LOST_FOCUS,
		WINDOW_MOVED,
		TICK,
		UPDATE,
		RENDER,
		KEY_PRESSED,
		KEY_RELEASED,
		KEY_TYPED,
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_MOVED_EVENT,
		MOUSE_SCROLLED,
		SCENE_PRE_START,
		SCENE_POST_START,
		SCENE_PRE_STOP,
		SCENE_POST_STOP
	};

	enum EventCategory
	{
		NONE = 0,
		EVENT_CATEGORY_APP =			BIT(0),
		EVENT_CATEGORY_INPUT =			BIT(1),
		EVENT_CATEGORY_KEYBOARD =		BIT(2),
		EVENT_CATEGORY_MOUSE =			BIT(3),
		EVENT_CATEGORY_MOUSE_BUTTON =	BIT(4),
		EVENT_CATEGORY_SCENE =	BIT(5),
	};


	class Event
	{
	public:
		Event(EventType type) : mType(type) {}

		virtual ~Event() = default;

		virtual void Invoke() {}
		virtual EventType GetType() { return mType; }
		virtual const EventType GetType() const { return mType; }
		virtual int GetCategoryFlags() const = 0;

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

		virtual std::string ToString() const = 0;

		bool handled = false;

	protected:
		EventType mType;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
	
}
