#pragma once

#include <Window.h>
#include <GLSLProgram.h>
#include <Camera2D.h>
#include <InputManager.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "BallManager.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;

enum class GameState{
    PLAY,
    EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();
    /// Renders Font
    void drawHud();

    void initLevel();


    /// Member Variables
    GameEngine::Window m_window; ///< The game window
    GameEngine::GLSLProgram m_textureProgram; ///< The shader program
    GameEngine::InputManager m_inputManager; ///< Handles input
    GameEngine::Camera2D m_camera; ///< Main Camera
    GameEngine::Camera2D m_hudCamera; ///< Main Camera
    GameEngine::SpriteBatch m_agentSpriteBatch;
    GameEngine::SpriteBatch m_hudSpriteBatch;
//    GameEngine::SpriteFont* _spriteFont;
    
    int m_screenWidth, m_screenHeight;

    GameState m_gameState;

    float m_fps;

    BallManager m_ballMan;
    
};
