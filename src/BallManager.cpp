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

void BallManager::init(int width, int height, const int n_masses){
    m_width = width;
    m_height = height;
    m_springtex = GameEngine::ResourceManager::getTexture("Textures/spring.png").id;
    m_masstex = GameEngine::ResourceManager::getTexture("Textures/circle.png").id;
    m_masscolor = GameEngine::ColorRGBA8(255,255,255,255);
    
    m_size = n_masses;
    m_masses = new mass_struct[m_size];
    m_springs = new spring_struct*[m_size];
    for (int i = 0; i<m_size; i++){
        m_springs[i] = new spring_struct[m_size];
    }
}

void BallManager::update(float dt){

  updateMasses();
  
  // Collisions
  //correctCollisions();
}

void BallManager::draw(GameEngine::SpriteBatch& sb){
    drawMasses(sb);
}

void BallManager::drawSprings(GameEngine::SpriteBatch& sb){
    for (int i = 0; i<m_size; i++){
        for (int j = 0; j<m_size; j++){
            if (m_springs[i][j].k<=0.0) continue;
            
            glm::vec3 difpos = (m_masses[j].pos - m_masses[i].pos);
            float dist = glm::length(difpos);
            glm::vec3 mid = m_masses[i].pos + (difpos*0.5f);
            glm::vec4 uvRect = glm::vec4(0.0f,0.0f,1.0f,1.0f);
            glm::vec4 destRect = glm::vec4(mid.x-dist/2,mid.y,dist,2);

            GLubyte R = 255; GLubyte B = 255;
            spring_struct s = m_springs[i][j];
            if (dist>s.l){ B = 0;}
            if (dist<s.l){ R = 0;}
            GameEngine::ColorRGBA8 color = GameEngine::ColorRGBA8(R,0,B,255);

            glm::vec2 drawpos = glm::vec2(difpos.x, difpos.y);
            sb.draw(destRect,uvRect,m_springtex,0.0f,color,glm::normalize(drawpos));
        }
    }
}

void BallManager::updateMasses(){
    for (int i = 0; i<m_size; i++){
      for (int j = i+1; j<m_size; j++){
            
            
            
            
      }
    }
}

void BallManager::correctCollisions(){
    for (int i = 0; i<m_size; i++){
        // outside walls
        if (m_masses[i].pos.x < -m_width){
            m_masses[i].pos.x = -m_width;
            m_masses[i].vel.x = -m_masses[i].vel.x;
        } else if (m_masses[i].pos.x > m_width) {
            m_masses[i].pos.x = m_width;
            m_masses[i].vel.x = -m_masses[i].vel.x;
        }
        if (m_masses[i].pos.y < -m_height){
            m_masses[i].pos.y = -m_height;
            m_masses[i].vel.y = -m_masses[i].vel.y;
        } else if (m_masses[i].pos.y > m_height) {
            m_masses[i].pos.y = m_height;
            m_masses[i].vel.y = -m_masses[i].vel.y;
        }
        
        for (int j = i+1; j<m_size; j++){
             // Check collision
            const float min_dist=(MASS_RADIUS + MASS_RADIUS); // r1 + r2
            const glm::vec3 difpos = m_masses[i].pos - m_masses[j].pos;
            const float dist = glm::length(difpos);
            if ( dist < min_dist){
                const float sum_mass = m_masses[i].mass+m_masses[j].mass;
                const glm::vec3 difvel = m_masses[i].vel-m_masses[j].vel;
                
                const double vel_dir = glm::dot(difvel,difpos);
                const double dif_len = glm::dot(difpos,difpos);
                const double r_vd = (vel_dir/dif_len);
                const double r_m1 = m_masses[j].mass/sum_mass;
                const double r_m2 = m_masses[i].mass/sum_mass;
                glm::vec3 newv1 = 
                        m_masses[i].vel - float(2*r_m1*r_vd)*(difpos);
                glm::vec3 newv2 = 
                        m_masses[j].vel + float(2*r_m2*r_vd)*(difpos);
                m_masses[i].vel = newv1;
                m_masses[j].vel = newv2;
                
                // Fix positions
                const glm::vec3 deltavec = ((min_dist/dist)-1)*difpos;
                m_masses[i].pos += deltavec * float(r_m2);
                m_masses[j].pos -= deltavec * float(r_m1);
            }
        }
     } 
}

void BallManager::drawMasses(GameEngine::SpriteBatch& sb){
    for (int i = 0; i<m_size; i++){
        mass_struct m = m_masses[i];
        glm::vec4 renderRect(0.0f);
        renderRect.x = (m.pos.x - MASS_RADIUS);
        renderRect.y = (m.pos.y - MASS_RADIUS);
        renderRect.w = 2*MASS_RADIUS;
        renderRect.z = 2*MASS_RADIUS;
        glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
        sb.draw(renderRect,uvRect,m_masstex,0.0f,m_masscolor);
    }
}

void BallManager::addMass(glm::vec3 pos, glm::vec3 vel, double mass, double charge){
    mass_struct m(pos,vel,mass,charge);
    m_masses[m_index] = m;
    
    for (unsigned int i = 0; i<m_size; i++){
        spring_struct s(0,0,0);
        m_springs[m_index][i] = s;
        m_springs[i][m_index] = s;
    }
    m_index++;
}

void BallManager::addSpring(int i, int j, double k, double l, double y){
    m_springs[i][j] = spring_struct(k,l,y);
    m_springs[j][i] = spring_struct(k,l,y);
    
    printf("%f\n",m_springs[i][j].l);
}
