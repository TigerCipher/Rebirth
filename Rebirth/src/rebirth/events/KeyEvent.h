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
// File Name: KeyEvent.h
// Date File Created: 06/14/2022 at 11:38 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


#include "Event.h"
#include "rebirth/input/InputCodes.h"

namespace rebirth
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return mKeyCode; }

		EVENT_CLASS_CATEGORY(EVENT_CATEGORY_KEYBOARD | EVENT_CATEGORY_INPUT)
	protected:
		KeyEvent(EventType type, KeyCode keycode)
			: Event(type), mKeyCode(keycode) {}

		KeyCode mKeyCode;
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, const int repeatCount)
			: KeyEvent(EventType::KEY_PRESSED, keycode), mRepeatCount(repeatCount) {}

		int GetRepeatCount() const { return mRepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << mKeyCode << " (" << mRepeatCount << " repeats)";
			return ss.str();
		}

		//EVENT_CLASS_TYPE(KEY_PRESSED)
	private:
		int mRepeatCount;
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode)
			: KeyEvent(EventType::KEY_RELEASED, keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << mKeyCode;
			return ss.str();
		}

		//EVENT_CLASS_TYPE(KEY_RELEASED)
	};
}
