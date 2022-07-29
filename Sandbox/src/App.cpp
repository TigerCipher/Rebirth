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
// File Name: App.cpp
// Date File Created: 06/13/2022 at 2:16 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include <Rebirth.h>
#include <rebirth/core/EntryPoint.h>

//#include "SampleLayer.h"
//#include "Sandbox2D.h"
#include "AssetLayer.h"


class Sandbox final : public rebirth::Application
{
public:
	Sandbox(rebirth::CommandLineArgs args) : Application({"Sandbox", 1920, 1080}, args)
	{
		//PushLayer(new SampleLayer());
		PushLayer(new rebirth::AssetLayer());
	}

	virtual ~Sandbox() = default;

};


rebirth::Application* rebirth::CreateApplication(rebirth::CommandLineArgs args)
{
	return new Sandbox(args);
}
