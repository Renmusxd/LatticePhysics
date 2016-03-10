#include "Window.h"
#include "Errors.h"

namespace GameEngine {

Window::Window(){}

Window::~Window(){}

void Window::swapBuffer(){
    SDL_GL_SwapWindow(m_sdlWindow);
}

int Window::create(const std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags){
    m_screenWidth = screenWidth; m_screenHeight = screenHeight;
    Uint32 flags = SDL_WINDOW_OPENGL;
    if (currentFlags & INVISIBLE){
            flags |= SDL_WINDOW_HIDDEN;
    }
    if (currentFlags & FULLSCREEN){
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    if (currentFlags & BORDERLESS){
            flags |= SDL_WINDOW_BORDERLESS;
    }

    m_sdlWindow = SDL_CreateWindow(
            windowName.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            screenWidth, screenHeight,
            flags);

    // Check if window made
    if (m_sdlWindow == NULL){
            fatalError("SDL Window could not be created");
    }

    SDL_GLContext glcontext = SDL_GL_CreateContext(m_sdlWindow);
    if (glcontext == NULL){
            fatalError("SDL context could not be initialized");
    }

    //GLenum glerror = glewInit();
    //if (glerror != GLEW_OK){
    //	fatalError("Could not initialize glew");
    //}


    glClearColor(1.0,1.0,1.0,1.0);

    // Set VSync
    SDL_GL_SetSwapInterval(1);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    int glVersionMajor;
    int glVersionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &glVersionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &glVersionMinor);
    printf("\n=== OpenGL Implementation ===\n");
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GL Version (Strict): %d.%d\n", glVersionMajor, glVersionMinor);
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return 0;
}

}