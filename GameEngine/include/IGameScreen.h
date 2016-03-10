/* 
 * File:   IGameScreen.h
 * Author: Sumner
 *
 * Created on June 19, 2015, 12:25 AM
 */

#pragma once

#include <cstddef>

#define SCREEN_INDEX_NO_SCREEN -1

namespace GameEngine{
    
    class IMainGame;
    
    enum class ScreenState{
        NONE, RUNNING, EXIT_APPLICATION,
        CHANGE_NEXT, CHANGE_PREVIOUS
    };
    
    class IGameScreen {
    public:
        friend class ScreenList;
        IGameScreen(){}
        virtual ~IGameScreen(){}
        // Called at beginning and end
        virtual void build() = 0;
        virtual void destroy() = 0;
        
        virtual void onEntry() = 0;
        virtual void onExit() = 0;
        
        virtual void update() = 0;
        virtual void draw() = 0;
        
        virtual int getNextScreenIndex() const = 0;
        virtual int getPreviousScreenIndex() const = 0;
        
        int getScreenIndex() const{return m_screenIndex;}
        
        void setParentGame(IMainGame* game){m_game = game;}
        
        ScreenState getScreenState() const {return m_currentState;}
        
        void setRunning(){m_currentState = ScreenState::RUNNING;}
        
    protected:
        ScreenState m_currentState = ScreenState::NONE;
        IMainGame* m_game = NULL;
        int m_screenIndex = -1;
    };
}


