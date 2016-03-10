/* 
 * File:   Ball.cpp
 * Author: Sumner
 * 
 * Created on June 24, 2015, 9:51 PM
 */
#include <ResourceManager.h>
#include "Ball.h"
#include <iostream>

Ball::Ball() {
    m_pos = glm::vec2(0.0f);
    m_vel = glm::vec2(0.0f);
    m_mass = 1.0;
    m_color = GameEngine::ColorRGBA8(255,255,255,255);
    m_textureid = GameEngine::ResourceManager::getTexture("Textures/circle.png").id;
}

Ball::~Ball() {
    
}



void Ball::update(float dt){
    for (Spring& s : m_springs){
        glm::vec2 dir = (s.target->m_pos - m_pos);
        glm::vec2 relv = (s.target->m_vel - m_vel);
        float dist = glm::length(dir);
        float f_dist = s.equilibrium - dist;
        // Friction
        glm::vec2 springrelv = glm::dot(relv,dir/dist) * (dir/dist);
        m_vel += springrelv * s.gamma;
        s.target->m_vel -= springrelv * s.gamma;
        // Force
        m_vel -= dir * (f_dist/dist) * s.hookval / m_mass;
        s.target->m_vel += dir * (f_dist/dist) * s.hookval / s.target->m_mass;
    }
}

void Ball::addSpring(Ball* b, float l, float k, float y){
    m_springs.emplace_back(b,l,k,y);
}

void Ball::draw(GameEngine::SpriteBatch& sb){
    glm::vec4 renderRect(0.0f);
    renderRect.x = (m_pos.x - m_radius);
    renderRect.y = (m_pos.y - m_radius);
    renderRect.w = 2*m_radius;
    renderRect.z = 2*m_radius;
    glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
    sb.draw(renderRect,uvRect,m_textureid,0.0f,m_color);
    
}