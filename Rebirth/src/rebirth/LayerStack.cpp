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

rebirth::LayerStack::LayerStack()
{
	mLayerInsert = mLayers.begin();
}

rebirth::LayerStack::~LayerStack()
{
	for(Layer* layer : mLayers)
	{
		delete layer;
	}
}

void rebirth::LayerStack::PushLayer(Layer* layer)
{
	mLayerInsert = mLayers.emplace(mLayerInsert, layer);
}

void rebirth::LayerStack::PushOverlay(Layer* overlay)
{
	mLayers.emplace_back(overlay);
}

void rebirth::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(mLayers.begin(), mLayers.end(), layer);

	if(it != mLayers.end())
	{
		mLayers.erase(it);
		--mLayerInsert;
	}
}




