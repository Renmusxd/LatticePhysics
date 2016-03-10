/* 
 * File:   BallManager.cpp
 * Author: Sumner
 * 
 * Created on June 24, 2015, 9:55 PM
 */
#include <GL/glew.h>
#include <ResourceManager.h>
#include "BallManager.h"
#include "Vertex.h"


BallManager::BallManager() {
}

BallManager::~BallManager() {
}

void BallManager::init(int width, int height, glm::vec2 gravity){
    m_width = width;
    m_height = height;
    m_springtex = GameEngine::ResourceManager::getTexture("Textures/spring.png").id;
    m_gravity = gravity;
}

void BallManager::update(float dt){
    for (Ball& b : m_balls){
        b.update(dt);
        
    }
    for (Ball& b : m_balls){
        b.m_pos += (b.m_vel * dt);
    }
    // Collisions
    correctCollisions();
}

void BallManager::draw(GameEngine::SpriteBatch& sb){
    for (auto& b : m_balls){
        b.draw(sb);
        b.m_vel += m_gravity;
    }
}

void BallManager::drawSprings(GameEngine::SpriteBatch& sb){
    for (const Ball& b : m_balls){
        for (const Spring& s : b.m_springs){
             glm::vec2 difpos = (s.target->m_pos - b.m_pos);
             float dist = glm::length(difpos);
             glm::vec2 mid = b.m_pos + (difpos*0.5f);
             glm::vec4 uvRect = glm::vec4(0.0f,0.0f,1.0f,1.0f);
             glm::vec4 destRect = glm::vec4(mid.x-dist/2,mid.y,dist,2);
             
             GLubyte R = 255; GLubyte B = 255;
             if (dist>s.equilibrium){ B = 0;}
             if (dist<s.equilibrium){ R = 0;}
             GameEngine::ColorRGBA8 color = GameEngine::ColorRGBA8(R,0,B,255);
             
            sb.draw(destRect,uvRect,m_springtex,0.0f,color,glm::normalize(difpos));
        }
    }
}

void BallManager::correctCollisions(){
    for (int i = 0; i<m_balls.size(); i++){
        // outside walls
        if (m_balls[i].m_pos.x < -m_width){
            m_balls[i].m_pos.x = -m_width;
            m_balls[i].m_vel.x = -m_balls[i].m_vel.x;
        } else if (m_balls[i].m_pos.x > m_width) {
            m_balls[i].m_pos.x = m_width;
            m_balls[i].m_vel.x = -m_balls[i].m_vel.x;
        }
        if (m_balls[i].m_pos.y < -m_height){
            m_balls[i].m_pos.y = -m_height;
            m_balls[i].m_vel.y = -m_balls[i].m_vel.y;
        } else if (m_balls[i].m_pos.y > m_height) {
            m_balls[i].m_pos.y = m_height;
            m_balls[i].m_vel.y = -m_balls[i].m_vel.y;
        }
        
        for (int j = i+1; j<m_balls.size(); j++){
             // Check collision
            const float min_dist=(m_balls[i].m_radius + m_balls[j].m_radius);
            const glm::vec2 difpos = m_balls[i].m_pos-m_balls[j].m_pos;
            const float dist = glm::length(difpos);
            if ( dist < min_dist){
                const float sum_mass = m_balls[i].m_mass+m_balls[j].m_mass;
                const glm::vec2 difvel = m_balls[i].m_vel-m_balls[j].m_vel;
                glm::vec2 newv1 = 
                        m_balls[i].m_vel - 
                        (2*m_balls[j].m_mass/(sum_mass))*
                        (glm::dot(difvel,difpos)/glm::dot(difpos,difpos))*
                        difpos;
                glm::vec2 newv2 = 
                        m_balls[j].m_vel - 
                        (2*m_balls[i].m_mass/(sum_mass))*
                        (glm::dot(difvel,difpos)/glm::dot(difpos,difpos))*
                        (-difpos);
                m_balls[i].m_vel = newv1;
                m_balls[j].m_vel = newv2;
                
                // Fix positions
                const glm::vec2 deltavec = ((min_dist/dist)-1)*difpos;
                m_balls[i].m_pos += (deltavec * (m_balls[i].m_mass/sum_mass));
                m_balls[j].m_pos -= (deltavec * (m_balls[j].m_mass/sum_mass));
            }
        }
     } 
}

void BallManager::addBall(glm::vec2 pos, glm::vec2 vel, float mass){
    m_balls.emplace_back();
    m_balls.back().init(pos,vel,mass);
}