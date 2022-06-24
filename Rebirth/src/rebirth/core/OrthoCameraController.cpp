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
#include "rebirth/input/KeyCodes.h"

namespace rebirth
{

	OrthoCameraController::OrthoCameraController(const float aspectRatio, const bool useRotation /*= false*/) :
		mAspectRatio(aspectRatio), mUseRotation(useRotation), mCamera(-mAspectRatio * mZoom, mAspectRatio* mZoom, -mZoom, mZoom)
	{

	}

	void OrthoCameraController::OnUpdate(Timestep ts)
	{
		// mul by 2 because 2 is currently default
		mCamSpeed = 2.0f * mZoom;
		if (Input::IsKeyPressed(RB_KEY_A))
		{
			mCamPos.x -= mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(RB_KEY_D))
		{
			mCamPos.x += mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(RB_KEY_W))
		{
			mCamPos.y += mCamSpeed * ts;
		}

		if (Input::IsKeyPressed(RB_KEY_S))
		{
			mCamPos.y -= mCamSpeed * ts;
		}

		if (mUseRotation)
		{
			if (Input::IsKeyPressed(RB_KEY_Q))
			{
				mCamRotation += mCamRotationSpeed * ts;
			}

			if (Input::IsKeyPressed(RB_KEY_E))
			{
				mCamRotation -= mCamRotationSpeed * ts;
			}

			mCamera.SetRotation(mCamRotation);
		}

		mCamera.SetPosition(mCamPos);
	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher disp(e);
		disp.Dispatch<MouseScrolledEvent>(BIND_EVENT_FUNC(OrthoCameraController::OnMouseScrolled));
		disp.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNC(OrthoCameraController::OnWindowResize));
	}

	bool OrthoCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		mZoom -= e.GetYOffset() * 0.25f;
		mZoom = std::max(mZoom, 0.25f);
		mCamera.SetProjection(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
		return false;
	}

	bool OrthoCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		mAspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		mCamera.SetProjection(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
		return false;
	}

}

