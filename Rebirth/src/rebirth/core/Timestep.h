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
// File Name: Timestep.h
// Date File Created: 6/22/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

namespace rebirth
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : mTime(time) {}

		// Returns time in seconds
		operator float() const { return mTime; }

		float Seconds() const { return mTime; }
		float Milliseconds() const { return mTime * 1000.0f; }
	private:
		float mTime;
	};
}

