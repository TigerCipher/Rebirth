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
// File Name: SceneCamera.h
// Date File Created: 6/27/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#pragma once

#include "rebirth/renderer/Camera.h"

namespace rebirth
{
	class SceneCamera : public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);

		void SetViewportSize(uint32 width, uint32 height);

		float GetOrthographicSize() const { return mOrthographicSize; }
		void SetOrthographicSize(const float size) { mOrthographicSize = size; RecalculateProjection(); }
	private:

		void RecalculateProjection();

		float mAspectRatio = 0.0f;
		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;

	};
}

