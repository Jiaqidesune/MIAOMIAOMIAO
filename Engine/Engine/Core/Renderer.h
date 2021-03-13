#pragma once

#include "../Math/Vector2.h"
#include "../Graphics/GfxTexture.h"

#include "Quad.h"
#include "Program.h"

namespace Rendering
{
	Program* LoadShaders(const std::string& vertFileName, const std::string& fragFileName);
	void GenTexture2D(GLuint& target, GLint internalformat, GLenum format, GLenum type, int width, int height, void* data);

	struct RenderOptions
	{
		RenderOptions()
		{
			maxDepth = 4;
			numTilesX = 4;
			numTilesY = 4;
			useEnvMap = false;
			intensity = 1.0f;
			WindowSize = Vector2(1280, 980);
			frameSize = WindowSize;
		}

		Vector2 WindowSize;
		Vector2 frameSize;

		int maxDepth;
		int numTilesX;
		int numTilesY;
		bool useEnvMap;
		float intensity;
	};

	class Scene;

	class Renderer
	{
	public:

	};
}