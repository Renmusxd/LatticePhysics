#pragma once

#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>
#include <string>

namespace GameEngine {
    
	enum WindowFlags{
		INVISIBLE = 0x1, 
		FULLSCREEN = 0x2,
		BORDERLESS = 0x4
	};

	class Window
	{
	public:
            Window();
            ~Window();

            int create(const std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

            int getScreenWidth(){return m_screenWidth;}
            int getScreenHeight(){return m_screenHeight;}

            void swapBuffer();

	private:
            
            SDL_Window* m_sdlWindow;
            int m_screenWidth;
            int m_screenHeight;
	};
}