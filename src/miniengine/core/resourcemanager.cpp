//
// Created by matheus on 18/09/2026.
//

#include "miniengine/core/resourcemanager.h"

namespace MiniEngine
{
	ResourceManager* ResourceManager::p_resource = nullptr;

	ResourceManager& ResourceManager::get()
    {
        return *p_resource;
    }
    } // MiniEngine