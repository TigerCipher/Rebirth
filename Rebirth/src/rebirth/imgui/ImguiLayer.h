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
// File Name: ImguiLayer.h
// Date File Created: 06/17/2022 at 10:11 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once
#include "rebirth/core/Layer.h"


namespace rebirth
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnImguiRender() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool block) { mBlockEvents = block; }
	
	private:
		bool mBlockEvents = true;
		float mTime = 0.0f;
		
	};
}
