#include "TextureCache.h"
#include "ImageLoader.h"
#include "GLTexture.h"
#include <iostream>

using namespace GameEngine;

TextureCache::TextureCache(){

}
TextureCache::~TextureCache(){

}

GLTexture TextureCache::getTexture(std::string texturePath){

// Lookup texture in map
//	std::map<std::string,GLTexture>::iterator mit = _textureMap.find(texturePath);
// same as
	auto mit = m_textureMap.find(texturePath);
// Check if in map
	if (mit == m_textureMap.end()){
		// Load and insert
		GLTexture newTexture = ImageLoader::loadPNG(texturePath);

		m_textureMap.insert( make_pair(texturePath,newTexture) );

		//std::cout << "Loaded new Texture!\n";
		return newTexture;
	}
	//std::cout << "Used Cached Texture!\n";
	return mit->second;
}