/* 
 * File:   ScreenList.cpp
 * Author: Sumner
 * 
 * Created on June 19, 2015, 12:38 AM
 */

#include "ScreenList.h"
#include "IGameScreen.h"
#include "IMainGame.h"

#include <iostream>

namespace GameEngine{

    ScreenList::ScreenList(IMainGame* game) {
        m_game = game;
    }

    ScreenList::~ScreenList() {
        destroy();
    }

    IGameScreen* ScreenList::moveNext(){
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN){
            m_currentScreenIndex = currentScreen->getNextScreenIndex();
        }
        return getCurrent();
    }
    
    IGameScreen* ScreenList::movePrevious(){
        IGameScreen* currentScreen = getCurrent();
        if (currentScreen->getNextScreenIndex() != SCREEN_INDEX_NO_SCREEN){
            m_currentScreenIndex = currentScreen->getPreviousScreenIndex();
        }
        return getCurrent();
    }

    void ScreenList::setScreen(int nextScreen){
        m_currentScreenIndex = nextScreen;
    }
    
    void ScreenList::addScreen(IGameScreen* newScreen){
        newScreen->m_screenIndex = m_screens.size();
        newScreen->build();
        newScreen->setParentGame(m_game);
        m_screens.push_back(newScreen);
        m_screens[0]->destroy();
    }

    void ScreenList::destroy(){
        for (auto i = 0; i<m_screens.size(); i++){
            m_screens[i]->destroy();
        }
        m_screens.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }
    
    IGameScreen* ScreenList::getCurrent(){
        if (m_currentScreenIndex==SCREEN_INDEX_NO_SCREEN){
            return NULL;
        }
        return m_screens[m_currentScreenIndex];
    }
    
}