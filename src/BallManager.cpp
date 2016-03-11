/* 
 * File:   BallManager.cpp
 * Author: Sumner
 * 
 * Created on June 24, 2015, 9:55 PM
 */
#include <cmath>
#include <GL/glew.h>
#include <ResourceManager.h>
#include "BallManager.h"
#include "Vertex.h"


BallManager::BallManager() {
}

BallManager::~BallManager() {
    
}

void BallManager::init(const int n_masses){
//    m_width = width;
//    m_height = height;
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

void BallManager::update(){
    // Collisions
    correctCollisions();
    updateMasses(0);
    m_hasCM = false;
    if (m_writeOutput){
        writeOutputToFile();
    }
}

void BallManager::writeOutputToFile(){
    
}

void BallManager::draw(GameEngine::SpriteBatch& sb){
    drawMasses(sb);
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

void BallManager::calcStep(float t, float* src, float* dest){
    memset(dest,0,sizeof(float)*6*m_size);
    for (int i = 0; i<m_size; i++){
        const int m_indx = 6*i;
        const float xi = src[m_indx];
        const float yi = src[m_indx+1];
        const float zi = src[m_indx+2];
        const float vxi = src[m_indx+3];
        const float vyi = src[m_indx+4];
        const float vzi = src[m_indx+5];
        
        dest[m_indx]   = src[m_indx+3];
        dest[m_indx+1] = src[m_indx+4];
        dest[m_indx+2] = src[m_indx+5];
        for (int j = i+1; j<m_size; j++){
            const int m_jndx = 6*j;
            const float xj = src[m_jndx];
            const float yj = src[m_jndx+1];
            const float zj = src[m_jndx+2];
            
            const float vxj = src[m_jndx+3];
            const float vyj = src[m_jndx+4];
            const float vzj = src[m_jndx+5];
            
            float d2 = (xj-xi)*(xj-xi) + 
                       (yj-yi)*(yj-yi) + 
                       (zj-zi)*(zj-zi);
            float d  = std::sqrt(d2);
            
            float vd2 = (vxj-vxi)*(vxj-vxi) + 
                       (vyj-vyi)*(vyj-vyi) + 
                       (vzj-vzi)*(vzj-vzi);
            float vd  = std::sqrt(vd2);
            
            const float r_x = ((xj-xi)/d);
            const float r_y = ((yj-yi)/d);
            const float r_z = ((zj-zi)/d);
            
            const float rv_x = ((vxj-vxi)/vd);
            const float rv_y = ((vyj-vyi)/vd);
            const float rv_z = ((vzj-vzi)/vd);
            
            /* Spring calculations */
            float hook_diff = m_springs[i][j].k * (d-m_springs[i][j].l);
            float spring_x = r_x * hook_diff;
            float spring_y = r_y * hook_diff;
            float spring_z = r_z * hook_diff;
            
            float spring_vx = 0;
            float spring_vy = 0;
            float spring_vz = 0;
            
            if (vd>0){
                float tx = rv_x * m_springs[i][j].y;
                float ty = rv_y * m_springs[i][j].y;
                float tz = rv_z * m_springs[i][j].y;
                spring_vx = (tx * r_x + ty*r_y + tz*r_z)*r_x;
                spring_vy = (tx * r_x + ty*r_y + tz*r_z)*r_y;
                spring_vz = (tx * r_x + ty*r_y + tz*r_z)*r_z;
            }
            /* Gravity calculations */
            float grav_pot = 0; 
            if (d2>0){
                grav_pot = -(m_masses[i].charge*m_masses[j].charge)/d2;
            }
            float grav_x = ((xj-xi)/d) * grav_pot;
            float grav_y = ((yj-yi)/d) * grav_pot;
            float grav_z = ((zj-zi)/d) * grav_pot;
            
            dest[m_indx+3] += spring_x + spring_vx + grav_x;
            dest[m_indx+4] += spring_y + spring_vy + grav_y;
            dest[m_indx+5] += spring_z + spring_vz + grav_z;
            dest[m_jndx+3] -= spring_x + spring_vx + grav_x;
            dest[m_jndx+4] -= spring_y + spring_vy + grav_y;
            dest[m_jndx+5] -= spring_z + spring_vz + grav_z;
        }
    }
}

void BallManager::updateMasses(float h){
    // Using rk4 with h-value
    const float t0 = 0;
    // override
    h = 0;
    
    float v[6*m_size];
    // Calc k0
    for (int i = 0; i<m_size; i++){
        v[6*i]   = m_masses[i].pos.x;
        v[6*i+1] = m_masses[i].pos.y;
        v[6*i+2] = m_masses[i].pos.z;
        v[6*i+3] = m_masses[i].vel.x;
        v[6*i+4] = m_masses[i].vel.y;
        v[6*i+5] = m_masses[i].vel.z;
    }
    float k0[6*m_size];
    calcStep(t0,v,k0);
    
    // Now calc k1
    for (int i = 0; i<m_size; i++){
        v[6*i]   = m_masses[i].pos.x + 0.5*k0[6*i];
        v[6*i+1] = m_masses[i].pos.y + 0.5*k0[6*i+1];
        v[6*i+2] = m_masses[i].pos.z + 0.5*k0[6*i+2];
        v[6*i+3] = m_masses[i].vel.x + 0.5*k0[6*i+3];
        v[6*i+4] = m_masses[i].vel.y + 0.5*k0[6*i+4];
        v[6*i+5] = m_masses[i].vel.z + 0.5*k0[6*i+5];
    }
    float k1[6*m_size];
    calcStep(t0 + 0.5*h,v,k1);
    
    // Now calc k2
    for (int i = 0; i<m_size; i++){
        v[6*i]   = m_masses[i].pos.x + 0.5*k1[6*i];
        v[6*i+1] = m_masses[i].pos.y + 0.5*k1[6*i+1];
        v[6*i+2] = m_masses[i].pos.z + 0.5*k1[6*i+2];
        v[6*i+3] = m_masses[i].vel.x + 0.5*k1[6*i+3];
        v[6*i+4] = m_masses[i].vel.y + 0.5*k1[6*i+4];
        v[6*i+5] = m_masses[i].vel.z + 0.5*k1[6*i+5];
    }
    float k2[6*m_size];
    calcStep(t0 + 0.5*h,v,k2);
    
    // Finally k3
    for (int i = 0; i<m_size; i++){
        v[6*i]   = m_masses[i].pos.x + k2[6*i];
        v[6*i+1] = m_masses[i].pos.y + k2[6*i+1];
        v[6*i+2] = m_masses[i].pos.z + k2[6*i+2];
        v[6*i+3] = m_masses[i].vel.x + k2[6*i+3];
        v[6*i+4] = m_masses[i].vel.y + k2[6*i+4];
        v[6*i+5] = m_masses[i].vel.z + k2[6*i+5];
    }
    float k3[6*m_size];
    calcStep(t0 + h,v,k3);
    
    // Now calculate the new y values
    for (int i = 0; i<m_size; i++){
        const int mi = 6*i;
        m_masses[i].pos.x += 
                (1.0/6.0)*(k0[mi] + 2*k1[mi] + 2*k2[mi] + k3[mi]);
        m_masses[i].pos.y += 
                (1.0/6.0)*(k0[mi+1] + 2*k1[mi+1] + 2*k2[mi+1] + k3[mi+1]);
        m_masses[i].pos.z += 
                (1.0/6.0)*(k0[mi+2] + 2*k1[mi+2] + 2*k2[mi+2] + k3[mi+2]);
        m_masses[i].vel.x += 
                (1.0/6.0)*(k0[mi+3] + 2*k1[mi+3] + 2*k2[mi+3] + k3[mi+3]);
        m_masses[i].vel.y += 
                (1.0/6.0)*(k0[mi+4] + 2*k1[mi+4] + 2*k2[mi+4] + k3[mi+4]);
        m_masses[i].vel.z += 
                (1.0/6.0)*(k0[mi+5] + 2*k1[mi+5] + 2*k2[mi+5] + k3[mi+5]);
    }
}

void BallManager::correctCollisions(){
    for (int i = 0; i<m_size; i++){        
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

glm::vec3 BallManager::getCM(){
    if (!m_hasCM){
        glm::vec3 cm(0,0,0);
        float total_mass = 0;
        for (int i = 0; i<m_size; i++){
            cm += m_masses[i].pos * m_masses[i].mass;
            total_mass += m_masses[i].mass;
        }
        m_cm = cm/total_mass;
    }
    return m_cm;
}

void BallManager::addMass(glm::vec3 pos, glm::vec3 vel, float mass, float charge){
    mass_struct m(pos,vel,mass,charge);
    m_masses[m_index] = m;
    
    for (unsigned int i = 0; i<m_size; i++){
        spring_struct s(0,0,0);
        m_springs[m_index][i] = s;
        m_springs[i][m_index] = s;
    }
    m_index++;
}

void BallManager::addSpring(int i, int j, float k, float l, float y){
    m_springs[i][j] = spring_struct(k,l,y);
    m_springs[j][i] = spring_struct(k,l,y);
}
