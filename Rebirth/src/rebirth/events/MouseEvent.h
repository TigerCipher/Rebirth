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
// File Name: MouseEvent.h
// Date File Created: 06/14/2022 at 11:40 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


#include "Event.h"


namespace rebirth
{
	class RB_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: mMouseX(x), mMouseY(y) {}

		[[nodiscard]] float GetX() const { return mMouseX; }
		[[nodiscard]] float GetY() const { return mMouseY; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << mMouseX << ", " << mMouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	private:
		float mMouseX;
		float mMouseY;
	};

	class RB_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: mXOffset(xOffset), mYOffset(yOffset) {}

		[[nodiscard]] float GetXOffset() const { return mXOffset; }
		[[nodiscard]] float GetYOffset() const { return mYOffset; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	private:
		float mXOffset;
		float mYOffset;
	};

	class RB_API MouseButtonEvent : public Event
	{
	public:
		[[nodiscard]] int GetMouseButton() const { return mButton; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_MOUSE | EVENT_CATEGORY_INPUT)
	protected:
		MouseButtonEvent(const int button)
			: mButton(button) {}

		int mButton;
	};

	class RB_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	class RB_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << mButton;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};
}
