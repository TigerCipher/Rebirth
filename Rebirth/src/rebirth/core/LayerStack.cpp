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
// File Name: LayerStack.cpp
// Date File Created: 06/16/2022 at 9:30 PM
// Author: Matt
// 
// ------------------------------------------------------------------------------

#include "rbpch.h"
#include "LayerStack.h"

namespace rebirth
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		mLayers.emplace(mLayers.begin() + mInsertIndex, layer);
		//layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		mLayers.emplace_back(overlay);
		//overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		if (const auto it = std::find(mLayers.begin() + mInsertIndex, mLayers.end(), layer); it != mLayers.begin() + mInsertIndex)
		{
			layer->OnDetach();
			mLayers.erase(it);
			--mInsertIndex;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		if (const auto it = std::find(mLayers.begin() + mInsertIndex, mLayers.end(), overlay); it != mLayers.end())
		{
			overlay->OnDetach();
			mLayers.erase(it);
		}
	}


}