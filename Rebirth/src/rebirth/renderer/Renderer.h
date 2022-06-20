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
// File Name: Renderer.h
// Date File Created: 06/19/2022 at 4:18 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------


#pragma once

namespace rebirth
{
	enum class GraphicsAPI
	{
		NONE = 0,
		OPENGL
	};


	class Renderer
	{
	public:
		static GraphicsAPI GetAPI() { return sGraphicsAPI; }
	private:
		static GraphicsAPI sGraphicsAPI;
	};
}