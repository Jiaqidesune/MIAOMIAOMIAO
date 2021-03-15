#pragma once

#include <string>

#include "../Core/Scene.h"

namespace Rendering
{
	bool LoadSceneFromFile(const std::string& filename, Scene* scene, RenderOptions& renderOptions);
}