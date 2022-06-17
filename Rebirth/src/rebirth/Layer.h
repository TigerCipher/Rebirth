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
// File Name: Layer.h
// Date File Created: 06/16/2022 at 9:24 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once

#include "Core.h"
#include "events/Event.h"
namespace rebirth
{
	class RB_API Layer
	{
	public:
		Layer(const std::string& name = "Layer") : mName(name) {}
		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate() {}

		virtual void OnEvent(Event& e) {}

	protected:
		std::string mName;
	};
}
