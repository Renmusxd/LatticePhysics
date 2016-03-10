#include <GameEngine.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <Timing.h>
#include <ctime>
#include <algorithm>
#include <random>
#include <Errors.h>
#include <ResourceManager.h>
#include <glm/gtx/rotate_vector.hpp>

#include "MainGame.h"

MainGame::MainGame() :
	m_screenWidth(500),
	m_screenHeight(500),
	m_gameState(GameState::PLAY),
	m_fps(0.0f)
{
    // Empty
}

MainGame::~MainGame() {

}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
}

void MainGame::initSystems() {
    // SDL
    GameEngine::init();

    m_window.create("Physics",m_screenWidth,m_screenHeight,0);
    glClearColor(0,0,0,1.0f);
    initShaders();

    // Init sprite batches
    m_agentSpriteBatch.init();
    m_hudSpriteBatch.init();

    // Init font (after sdl init)
//    _spriteFont = new GameEngine::SpriteFont("Fonts/chintzy.ttf", 32);

    m_camera.init(m_screenWidth, m_screenHeight);
    m_hudCamera.init(m_screenWidth, m_screenHeight);
    m_hudCamera.setPosition(glm::vec2(m_screenWidth/2,m_screenHeight/2));
}

void MainGame::initShaders() {
    // Compile our color shader
    m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
    const float DESIRED_FPS = 60.0f;
    const int MAX_PHYSICS_STEPS = 6;
    const float CAMERA_SCALE = 1.0 / 2.0f;
    m_camera.setScale(CAMERA_SCALE);
    GameEngine::FpsLimiter fpsLimiter;
    fpsLimiter.setMaxFPS(DESIRED_FPS);

    const float MS_PER_SECOND = 1000;
    const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;
    const float MAX_DELTA_TIME = 1.0f;

    float prevTicks = SDL_GetTicks();

    while (m_gameState == GameState::PLAY){
        fpsLimiter.begin();
        float newTicks = SDL_GetTicks();
        float frameTime = newTicks - prevTicks;
        prevTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

        m_inputManager.update();
        processInput();

        int i = 0;
        while (totalDeltaTime > 0.0f && i<MAX_PHYSICS_STEPS){
            float deltatime = std::min(totalDeltaTime, MAX_DELTA_TIME);

            m_ballMan.update(deltatime);
            // call update balls with deltatime
            totalDeltaTime -= deltatime;
            i++;
        }


//  	_camera.setPosition(_player->getPosition());
        m_camera.update();
        m_hudCamera.update();
        drawGame();

        m_fps = fpsLimiter.end();
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
            	m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_textureProgram.use();

    glActiveTexture(GL_TEXTURE0);
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform,1,GL_FALSE, &projectionMatrix[0][0]);

   m_agentSpriteBatch.begin();

   
   m_ballMan.draw(m_agentSpriteBatch);
   m_ballMan.drawSprings(m_agentSpriteBatch);
	// Draw humans
//	for (int i = 0; i<_humans.size(); i++){
//		if (_camera.isBoxInView(_humans[i]->getPosition(),agentDims)){
//			_humans[i]->draw(_agentSpriteBatch);
//		}
//	}

   m_agentSpriteBatch.end();
   m_agentSpriteBatch.renderBatch();
   
//   drawHud();
    m_textureProgram.unuse();
   

    // Swap our buffer and draw everything to the screen!
    m_window.swapBuffer();
}

void MainGame::drawHud(){
//    char buffer[256]; // bigger than needed

    glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform,1,GL_FALSE, &projectionMatrix[0][0]);
    
    m_hudSpriteBatch.begin();

//    std::sprintf(buffer, "Num Humans %lu", _humans.size());
//    _spriteFont->draw(
//            _hudSpriteBatch,					// To
//            buffer,						// From
//            glm::vec2(0,10),					// Where
//            glm::vec2(1.0),                                   // Size
//            0.0f,						// Depth
//            GameEngine::ColorRGBA8(0,0,0,255)                 // Tint
//            );

    m_hudSpriteBatch.end();
    m_hudSpriteBatch.renderBatch();
}

void MainGame::initLevel(){
    m_ballMan.init(m_screenWidth, m_screenHeight,glm::vec2(0,-0.1f));
    
    m_ballMan.addBall(glm::vec2(-100,-100),glm::vec2(0,25),10); // bl 0
    m_ballMan.addBall(glm::vec2(-100,100),glm::vec2(0,25),10);  // tl 1
    m_ballMan.addBall(glm::vec2(100,100),glm::vec2(0,25),10);   // tr 2
    m_ballMan.addBall(glm::vec2(100,-100),glm::vec2(0,25),10);  // br 3
    
    float offset = 200;
    
    m_ballMan.addBall(glm::vec2(offset-100,offset-100),glm::vec2(0,25),10); // bl 4
    m_ballMan.addBall(glm::vec2(offset-100,offset+100),glm::vec2(0,25),10); // tl 5
    m_ballMan.addBall(glm::vec2(offset+100,offset+100),glm::vec2(0,25),10); // tr 6
    m_ballMan.addBall(glm::vec2(offset+100,offset-100),glm::vec2(0,25),10); // br 7
    
    const float k = 0.5f;
    const float y = 0.05f;
        
    m_ballMan.m_balls[0].addSpring(&m_ballMan.m_balls[1],250,k,y);
    m_ballMan.m_balls[1].addSpring(&m_ballMan.m_balls[2],250,k,y);
    m_ballMan.m_balls[2].addSpring(&m_ballMan.m_balls[3],250,k,y);
    m_ballMan.m_balls[3].addSpring(&m_ballMan.m_balls[0],250,k,y);
    m_ballMan.m_balls[0].addSpring(&m_ballMan.m_balls[2],353,k,y);
    m_ballMan.m_balls[1].addSpring(&m_ballMan.m_balls[3],353,k,y);
    
    m_ballMan.m_balls[4].addSpring(&m_ballMan.m_balls[5],250,k,y);
    m_ballMan.m_balls[5].addSpring(&m_ballMan.m_balls[6],250,k,y);
    m_ballMan.m_balls[6].addSpring(&m_ballMan.m_balls[7],250,k,y);
    m_ballMan.m_balls[7].addSpring(&m_ballMan.m_balls[4],250,k,y);
    m_ballMan.m_balls[4].addSpring(&m_ballMan.m_balls[6],353,k,y);
    m_ballMan.m_balls[5].addSpring(&m_ballMan.m_balls[7],353,k,y);
}



