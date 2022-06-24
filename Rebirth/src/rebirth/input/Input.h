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
// File Name: Input.h
// Date File Created: 06/18/2022 at 4:02 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


#include "rebirth/Core.h"

namespace rebirth
{
	class RB_API Input
	{
	public:

		static bool IsKeyPressed(const int keycode)
		{
			return sInstance->IsKeyPressedImpl(keycode);
		}

		static bool IsMouseButtonPressed(const int button)
		{
			return sInstance->IsMouseButtonPressedImpl(button);
		}

		static float GetMouseX()
		{
			return sInstance->GetMouseXImpl();
		}

		static float GetMouseY()
		{
			return sInstance->GetMouseYImpl();
		}

		static std::pair<float, float> GetMousePos()
		{
			return sInstance->GetMousePosImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePosImpl() = 0;
		
	private:

		static Scope<Input> sInstance;
		
	};
}