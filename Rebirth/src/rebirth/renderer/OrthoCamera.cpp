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
// File Name: OrthoCamera.cpp
// Date File Created: 6/21/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"

#include "OrthoCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace rebirth
{

	OrthoCamera::OrthoCamera(float left, float right, float bottom, float top) :
		mProjection(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), mView(1.0f)
	{
		RB_PROFILE_FUNC();
		mViewProjection = mProjection * mView;
	}

	void OrthoCamera::SetProjection(float left, float right, float bottom, float top)
	{
		RB_PROFILE_FUNC();
		mProjection = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		mViewProjection = mProjection * mView;
	}

	void OrthoCamera::SetPosition(const glm::vec3& pos)
	{
		mPosition = pos;
		RecalculateViewMatrix();
	}


	void OrthoCamera::SetRotation(const float rot)
	{
		mRotation = rot;
		RecalculateViewMatrix();
	}

	void OrthoCamera::RecalculateViewMatrix()
	{
		RB_PROFILE_FUNC();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition) * glm::rotate(glm::mat4(1.0f), glm::radians(mRotation), glm::vec3(0, 0, 1));
		mView = glm::inverse(transform);
		mViewProjection = mProjection * mView;
	}

}

