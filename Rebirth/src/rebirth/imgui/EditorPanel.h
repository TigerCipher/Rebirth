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
// File Name: EditorPanel.h
// Date File Created: 7/17/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/core/Common.h"
#include "rebirth/scene/Scene.h"
#include "rebirth/events/Event.h"
#include "rebirth/events/EventListener.h"
#include "rebirth/core/Application.h"

namespace rebirth
{
	class EditorPanel : public EventListener
	{
	public:
		EditorPanel()
		{
			Application::Instance().GetEventDispatcher().AddListener(this);
		}
		virtual ~EditorPanel() = default;

		virtual void OnImguiRender() = 0;
		virtual void SetContext(const Ref<Scene>& context) {}
		virtual void OnEvent(Event& e) {}
	};

}

