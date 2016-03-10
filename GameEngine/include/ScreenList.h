/* 
 * File:   ScreenList.h
 * Author: Sumner
 *
 * Created on June 19, 2015, 12:38 AM
 */

#pragma once
#include <vector>

namespace GameEngine{
    class IMainGame;
    class IGameScreen;
    
    class ScreenList {
    public:
        ScreenList(IMainGame* game);
        virtual ~ScreenList();
        
        IGameScreen* moveNext();
        IGameScreen* movePrevious();
        
        void setScreen(int nextScreen);
        void addScreen(IGameScreen* newScreen);
        
        void destroy();
        
        IGameScreen* getCurrent();
        
    protected:
        std::vector<IGameScreen*> m_screens;
        int m_currentScreenIndex = -1;
        IMainGame* m_game;

    };

}

