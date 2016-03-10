#include "ResourceManager.h"
#include "GLTexture.h"

using namespace GameEngine;

TextureCache ResourceManager::m_textureCache;

GLTexture ResourceManager::getTexture(std::string texturePath){
	return m_textureCache.getTexture(texturePath);
}