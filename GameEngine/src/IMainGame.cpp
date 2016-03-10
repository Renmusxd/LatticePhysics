/* 
 * File:   MainGame.cpp
 * Author: Sumner
 * 
 * Created on June 19, 2015, 12:37 AM
 */

#include "IMainGame.h"
#include "IGameScreen.h"
#include "Timing.h"
#include "ScreenList.h"
#include <memory>

namespace GameEngine{
    IMainGame::IMainGame() {
        m_screenList = std::unique_ptr<ScreenList>(new ScreenList(this));
    }

    IMainGame::~IMainGame() {
    
    }

    void IMainGame::run(){
        if (!init()){
            return;
        }
        FpsLimiter limiter;
        limiter.setMaxFPS(60.0f);
        m_isRunning = true;
        while (m_isRunning){
            limiter.begin();
            
            update();
            if (m_isRunning){
                draw();
                m_fps = limiter.end();
                m_window.swapBuffer();
            }
        }
    }
    void IMainGame::exitGame(){
        m_currentScreen->onExit();
        if (m_screenList){
            m_screenList->destroy();
            m_screenList.reset();
        }
        m_isRunning = false;
    }
    
    bool IMainGame::init(){
        GameEngine::init();
        if (!initSystems()){ return false; }
        onInit();
        addScreens();
        m_currentScreen = m_screenList->getCurrent();
        m_currentScreen->onEntry();
        m_currentScreen->setRunning();
        return true;
    }
    bool IMainGame::initSystems(){
        m_window.create("Default",500,500,0);
        return true;
    }
    
    void IMainGame::onSDLEvent(SDL_Event& evnt){
        switch (evnt.type) {
            case SDL_QUIT:
                // Exit the game here!
                exitGame();
                m_isRunning = false;
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
    
    void IMainGame::update(){
        m_inputManager.update();
        if (m_currentScreen){
            switch (m_currentScreen->getScreenState()){
                case ScreenState::RUNNING:
                    m_currentScreen->update();
                    break;
                case ScreenState::CHANGE_NEXT:
                    m_currentScreen->onExit();
                    m_currentScreen = m_screenList->moveNext();
                    if (m_currentScreen){
                        m_currentScreen->onEntry();
                    }
                    break;
                case ScreenState::CHANGE_PREVIOUS:
                    m_currentScreen->onExit();
                    m_currentScreen = m_screenList->movePrevious();
                    if (m_currentScreen){
                        m_currentScreen->onEntry();
                    }
                    break;
                case ScreenState::EXIT_APPLICATION:
                    exitGame();
                    break;
                default:
                    break;
            }
        } else {
            exitGame();
        }
    }
    
    void IMainGame::draw(){
        glViewport(0,0,m_window.getScreenWidth(),m_window.getScreenHeight());
        if (m_currentScreen && m_currentScreen->getScreenState()==ScreenState::RUNNING){
            m_currentScreen->draw();
        } else {
            exitGame();
        }
    }
}
