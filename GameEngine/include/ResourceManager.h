#pragma once

#include <string>
#include "TextureCache.h"
#include "GLTexture.h"
namespace GameEngine {
	class ResourceManager{
	public:
		static GLTexture getTexture(std::string texturePath);
	private:
		static TextureCache m_textureCache;
	};
}