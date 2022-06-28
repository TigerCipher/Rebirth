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
		enum class ProjectionType
		{
			PERSPECTIVE = 0, ORTHOGRAPHIC = 1
		};
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);

		void SetViewportSize(uint32 width, uint32 height);

		void SetOrthographicSize(const float size) { mOrthographicSize = size; RecalculateProjection(); }
		void SetOrthographicNearClip(float nearClip) { mOrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthographicFarClip(float farClip) { mOrthographicFar = farClip; RecalculateProjection(); }
		float GetOrthographicSize() const { return mOrthographicSize; }
		float GetOrthographicNearClip() const { return mOrthographicNear; }
		float GetOrthographicFarClip() const { return mOrthographicFar; }

		void SetPerspectiveFoV(const float fov) { mPerspectiveFoV = fov; RecalculateProjection(); }
		void SetPerspectiveNearClip(float nearClip) { mPerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { mPerspectiveFar = farClip; RecalculateProjection(); }
		float GetPerspectiveFoV() const { return mPerspectiveFoV; }
		float GetPerspectiveNearClip() const { return mPerspectiveNear; }
		float GetPerspectiveFarClip() const { return mPerspectiveFar; }

		void SetProjectionType(ProjectionType type) { mProjectionType = type; RecalculateProjection(); }
		ProjectionType GetProjectionType() const { return mProjectionType; }
	private:

		void RecalculateProjection();

		ProjectionType mProjectionType = ProjectionType::ORTHOGRAPHIC; // #FUTURE switch to perspective when I implement 3D
		float mAspectRatio = 0.0f;

		float mOrthographicSize = 10.0f;
		float mOrthographicNear = -1.0f;
		float mOrthographicFar = 1.0f;


		float mPerspectiveFoV = glm::radians(45.0f);
		float mPerspectiveNear = 0.01f;
		float mPerspectiveFar = 1000.0f;

	};
}

