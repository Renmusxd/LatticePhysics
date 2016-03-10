#pragma once
//#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include "GLTexture.h"
#include <string>
namespace GameEngine {
	class Sprite{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);

		void draw();

	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;
		GLuint m_vboID;

		GLTexture m_texture;

	};
}