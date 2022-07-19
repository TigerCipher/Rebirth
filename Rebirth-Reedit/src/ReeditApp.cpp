// ------------------------------------------------------------------------------
// 
// Sandbox
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
// File Name: ReeditApp.cpp
// Date File Created: 06/26/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include <Rebirth.h>
#include <rebirth/core/EntryPoint.h>

#include "EditorLayer.h"

namespace rebirth
{
	class ReeditApp final : public Application
	{
	public:
		ReeditApp(ApplicationDesc appDesc, CommandLineArgs args) : Application(appDesc, args)
		{
			PushLayer(new EditorLayer());
		}
		~ReeditApp() override = default;

	};


	Application* CreateApplication(CommandLineArgs args)
	{
		ApplicationDesc desc;
		desc.title = "Rebirth Reedit";
		desc.windowWidth = 2560;
		desc.windowHeight = 1440;
		desc.flags |= WindowFlag_Maximized;
		desc.flags |= WindowFlag_VSync;
		return new ReeditApp(desc, args);
	}
}
