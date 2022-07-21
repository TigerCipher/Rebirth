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
// File Name: OrthoCameraController.cpp
// Date File Created: 6/23/2022
// Author: Matt
// 
// ------------------------------------------------------------------------------
#include "rbpch.h"
#include "OrthoCameraController.h"

#include "rebirth/input/Input.h"
#include "rebirth/input/InputCodes.h"

namespace rebirth
{

	OrthoCameraController::OrthoCameraController(const float aspectRatio, const bool useRotation /*= false*/) :
		mAspectRatio(aspectRatio), mUseRotation(useRotation),
		mBounds({ -mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom }),
		mCamera(mBounds.left, mBounds.right, mBounds.bottom, mBounds.top)
	{

	}

	void OrthoCameraController::OnUpdate(Timestep ts)
	{
		RB_PROFILE_FUNC();
		// mul by 2 because 2 is currently default
		mCamSpeed = 2.0f * mZoom;
		if (Input::IsKeyPressed(KeyCode::A))
		{
			mCamPos.x -= cos(glm::radians(mCamRotation)) * mCamSpeed * ts;
			mCamPos.y -= sin(glm::radians(mCamRotation)) * mCamSpeed * ts;
			//mCamPos.x -= mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(KeyCode::D))
		{
			mCamPos.x += cos(glm::radians(mCamRotation)) * mCamSpeed * ts;
			mCamPos.y += sin(glm::radians(mCamRotation)) * mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(KeyCode::W))
		{
			mCamPos.x += -sin(glm::radians(mCamRotation)) * mCamSpeed * ts;
			mCamPos.y += cos(glm::radians(mCamRotation)) * mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(KeyCode::S))
		{
			mCamPos.x -= -sin(glm::radians(mCamRotation)) * mCamSpeed * ts;
			mCamPos.y -= cos(glm::radians(mCamRotation)) * mCamSpeed * ts;
		}

		if (mUseRotation)
		{
			if (Input::IsKeyPressed(KeyCode::Q))
			{
				mCamRotation += mCamRotationSpeed * ts;
			}

			if (Input::IsKeyPressed(KeyCode::E))
			{
				mCamRotation -= mCamRotationSpeed * ts;
			}

			if (mCamRotation > 180.0f) mCamRotation -= 360.0f;
			if (mCamRotation <= -180.0f) mCamRotation += 360.0f;

			mCamera.SetRotation(mCamRotation);
		}

		mCamera.SetPosition(mCamPos);
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		RB_PROFILE_FUNC();

		if (e.GetType() == EventType::MOUSE_SCROLLED)
			OnMouseScrolled((MouseScrolledEvent&)e);
		else if (e.GetType() == EventType::WINDOW_RESIZE)
			OnWindowResize((WindowResizeEvent&)e);

	}

	void OrthoCameraController::ResizeBounds(float width, float height)
	{
		mAspectRatio = width / height;
		CalculateView();
	}

	void OrthoCameraController::CalculateView()
	{
		mBounds = { -mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom };
		mCamera.SetProjection(mBounds.left, mBounds.right, mBounds.bottom, mBounds.top);
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		RB_PROFILE_FUNC();
		mZoom -= e.GetYOffset() * 0.25f;
		mZoom = std::max(mZoom, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthoCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		RB_PROFILE_FUNC();
		ResizeBounds((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

}

