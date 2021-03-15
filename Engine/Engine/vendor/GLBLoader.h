#pragma once

#include <string>

#include "../core/Scene.h"
#include "../core/Renderer.h"

namespace Rendering
{
	bool LoadSceneFromGLTF(const std::string& filename, Scene* scene);
}