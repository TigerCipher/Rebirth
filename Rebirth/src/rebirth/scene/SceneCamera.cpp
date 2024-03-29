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
// File Name: SceneCamera.cpp
// Date File Created: 6/28/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "SceneCamera.h"

namespace rebirth
{

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		if (mProjectionType != ProjectionType::ORTHOGRAPHIC || mOrthographicSize != size || mOrthographicNear != nearClip || mOrthographicFar != farClip)
		{
			mProjectionType = ProjectionType::ORTHOGRAPHIC;
			mOrthographicSize = size;
			mOrthographicNear = nearClip;
			mOrthographicFar = farClip;
			RecalculateProjection();
		}
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		if (mProjectionType != ProjectionType::PERSPECTIVE || mPerspectiveFoV != fov || mPerspectiveNear != nearClip || mPerspectiveFar != farClip)
		{
			mProjectionType = ProjectionType::PERSPECTIVE;
			mPerspectiveFoV = fov;
			mPerspectiveNear = nearClip;
			mPerspectiveFar = farClip;
			RecalculateProjection();
		}
	}

	void SceneCamera::SetViewportSize(uint32 width, uint32 height)
	{
		RB_CORE_ASSERT(width > 0 && height > 0);
		float newApsect = (float)width / (float)height;
		if (mAspectRatio != newApsect)
		{
			mAspectRatio = newApsect;
			RecalculateProjection();
		}

	}

	void SceneCamera::RecalculateProjection()
	{
		if (mProjectionType == ProjectionType::PERSPECTIVE)
		{
			mProjection = glm::perspective(mPerspectiveFoV, mAspectRatio, mPerspectiveNear, mPerspectiveFar);
		}
		else
		{
			float orthoLeft = -mOrthographicSize * mAspectRatio * 0.5f;
			float orthoRight = mOrthographicSize * mAspectRatio * 0.5f;
			float orthoBottom = -mOrthographicSize * 0.5f;
			float orthoTop = mOrthographicSize * 0.5f;

			mProjection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, mOrthographicNear, mOrthographicFar);
		}
		
	}

}

