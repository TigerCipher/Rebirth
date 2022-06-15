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

#include "../Core.h"

#include <functional>
#include <string>



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
		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_MOVED,
		MOUSE_SCROLLED
	};

	enum EventCategory
	{
		NONE = 0,
		EVENT_CATEGORY_APP = BIT(0),
		EVENT_CATEGORY_INPUT = BIT(1),
		EVENT_CATEGORY_KEYBOARD = BIT(2),
		EVENT_CATEGORY_MOUSE = BIT(3),
		EVENT_CATEGORY_MOUSE_BUTTON = BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }					\
									virtual EventType GetEventType() const override { return GetStaticType(); }		\
									virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class RB_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		bool mHandled = false;
	};


	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : mEvent(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if(mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.mHandled = func(*(T*)&mEvent);
				return true;
			}

			return false;
		}

	private:
		Event& mEvent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
	
}
