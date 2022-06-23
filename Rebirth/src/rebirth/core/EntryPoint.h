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
// File Name: EntryPoint.h
// Date File Created: 06/13/2022 at 3:17 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#pragma once


#ifdef RB_WINDOWS

extern rebirth::Application* rebirth::CreateApplication();

int main(int argc, char** argv)
{
	rebirth::Log::Init();
	RB_CORE_TRACE("Initializing engine");

	auto app = rebirth::CreateApplication();
	app->Run();
	delete app;

	// TODO: Might be nice to return error codes for debug purposes
	// though... during debugging, I'd just assert instead of return an error code. Error code for release/dist builds?
	return 0;
}

#endif
