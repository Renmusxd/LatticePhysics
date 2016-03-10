/* 
 * File:   BallManager.h
 * Author: Sumner
 *
 * Created on June 24, 2015, 9:55 PM
 */

#include "Ball.h"
#include <vector>
#include <glm/glm.hpp>
#include <SpriteBatch.h>

#ifndef BALLMANAGER_H
#define	BALLMANAGER_H

class BallManager {
public:
    BallManager();
    virtual ~BallManager();
    
    void init(int width, int height, glm::vec2 gravity = glm::vec2(0.0f,0.0f));
    
    void update(float dt);
    void draw(GameEngine::SpriteBatch& sb);
    void drawSprings(GameEngine::SpriteBatch& sb);
    void addBall(glm::vec2 pos, glm::vec2 vel, float mass);
    
        
    std::vector<Ball> m_balls;
    
private:
    void correctCollisions();
    
    GLuint m_springtex;
    
    int m_width;
    int m_height;
    glm::vec2 m_gravity;
};

#endif	/* BALLMANAGER_H */

