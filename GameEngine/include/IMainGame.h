/* 
 * File:   MainGame.h
 * Author: Sumner
 *
 * Created on June 19, 2015, 12:37 AM
 */

#pragma once

#include "GameEngine.h"
#include "InputManager.h"
#include "Window.h"
#include "ScreenList.h"
#include <cstddef>
#include <memory>

namespace GameEngine{
    class IGameScreen;
    
    class IMainGame {
    public:
        IMainGame();
        virtual ~IMainGame();
        
        void run();
        void exitGame();
        
        virtual void onInit() = 0;
        virtual void addScreens() = 0;
        virtual void onExit() = 0;
        //virtual void addScreens() = 0;
        
        const float getFPS() const {return m_fps;}
        
        void onSDLEvent(SDL_Event& evnt);
        
    protected:
        virtual void update();
        virtual void draw();
      
        
        bool init();
        bool initSystems();
        
        InputManager m_inputManager;
        std::unique_ptr<ScreenList> m_screenList = NULL;
        IGameScreen* m_currentScreen = NULL;
        bool m_isRunning = false;
        float m_fps = 0;
        Window m_window;
    };

}


