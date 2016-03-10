/* 
 * File:   Ball.h
 * Author: Sumner
 *
 * Created on June 24, 2015, 9:51 PM
 */

#include <glm/glm.hpp>
#include <vector>
#include <SpriteBatch.h>
#include <OpenGL/gl3.h>
#include <Vertex.h>

#ifndef BALL_H
#define	BALL_H

const float MASS_MULT = 50;

struct Spring;
class Ball {
public:
    friend class BallManager;
    Ball();
    virtual ~Ball();
    // Update based on springs etc...
    void init(glm::vec2 pos, glm::vec2 vel, float mass){
        m_pos = pos; m_vel = vel; m_mass = mass;
        m_radius = std::cbrt(mass*MASS_MULT);
    }
    void update(float dt);
    void draw(GameEngine::SpriteBatch& sb);
    void setPosVel(glm::vec2 pos, glm::vec2 vel){m_pos=pos;m_vel=vel;}
    void addSpring(Ball* b, float l, float k, float y);
    
    std::vector<Spring> m_springs;
    glm::vec2 m_pos;
    glm::vec2 m_vel;
    float m_mass;
    float m_radius;
private:
    GLuint m_textureid;
    GameEngine::ColorRGBA8 m_color;
};

struct Spring{
    float hookval;
    float equilibrium;
    float gamma;
    Ball* target;
    Spring(Ball* b, float l, float k, float y){
        target = b; 
        equilibrium = l;
        hookval = k;
        gamma = y;
    }
};


#endif	/* BALL_H */

