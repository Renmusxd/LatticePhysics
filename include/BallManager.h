/* 
 * File:   BallManager.h
 * Author: Sumner
 *
 * Created on June 24, 2015, 9:55 PM
 */

#include <vector>
#include <glm/glm.hpp>
#include <SpriteBatch.h>

#ifndef BALLMANAGER_H
#define	BALLMANAGER_H

#define MASS_RADIUS 5

struct spring_struct{
    double k;
    double l;
    double y;
    spring_struct(double a, double b, double c){k=a;l=b;y=c;}
    spring_struct(){k=0;l=0;y=0;}
};

struct mass_struct{
    glm::vec3 pos;
    glm::vec3 vel;
    double mass = 1;
    double charge = 0;
    mass_struct(glm::vec3 p, glm::vec3 v, double m, double q){
        pos = p; vel = v; mass = m; charge = q;
    }
    mass_struct(){
        pos = glm::vec3(0,0,0);vel = glm::vec3(0,0,0);
        mass = 0; charge = 0;
    }
};

class BallManager {
public:
    BallManager();
    virtual ~BallManager();
    
    void init(int width, int height, const int n_masses);
    
    void update(float dt);
    void updateMasses();
    void draw(GameEngine::SpriteBatch& sb);
    void drawSprings(GameEngine::SpriteBatch& sb);
    
    void drawMasses(GameEngine::SpriteBatch& sb);
    void addMass(glm::vec3 pos, glm::vec3 vel, double mass, double charge);
    void addSpring(int i, int j, double k, double l, double y);
    
private:
    void correctCollisions();
    
    GLuint m_springtex;
    GLuint m_masstex;
    GameEngine::ColorRGBA8 m_masscolor;
    
    int m_width;
    int m_height;
    
    int m_size;
    int m_index = 0;
    
    mass_struct* m_masses;
    spring_struct** m_springs;
};

#endif	/* BALLMANAGER_H */

