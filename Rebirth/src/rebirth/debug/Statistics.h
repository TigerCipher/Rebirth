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
// File Name: Statistics.h
// Date File Created: 7/1/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once


namespace rebirth
{
	class Statistics
	{
	public:
		struct RenderStats
		{
			float frameTime = 0;
			int fps = 0;
		};
	public:
		static void SetFrameTime(float frameTime) { sRenderStats.frameTime = frameTime; }
		static void SetFPS(int fps) { sRenderStats.fps = fps; }

		static const RenderStats& GetRenderStats() { return sRenderStats; }
	private:
		

		static RenderStats sRenderStats;
	};
}


