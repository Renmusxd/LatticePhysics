#include <SDL2/SDL.h>
#include <OpenGL/gl3.h>

#include "GameEngine.h"

namespace GameEngine{
	int init(){
		//Request OpenGL 3.2 context.
		SDL_Init(SDL_INIT_EVERYTHING);	

		// Mac specific
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
		return 0;
	}
}