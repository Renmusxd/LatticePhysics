#include <vector>
#include <glm/glm.hpp>
#include <SpriteBatch.h>
#include <fstream>

#ifndef BALLMANAGER_H
#define	BALLMANAGER_H

#define MASS_RADIUS 5

struct spring_struct{
    float k;
    float l;
    float y;
    spring_struct(float a, float b, float c){k=a;l=b;y=c;}
    spring_struct(){k=0;l=0;y=0;}
};

struct mass_struct{
    glm::vec3 pos;
    glm::vec3 vel;
    float mass = 1;
    float pos_r = 0;
    float neg_r = 0;
    float pos_r2 = 0;
    float neg_r2 = 0;
//    mass_struct(glm::vec3 p, glm::vec3 v, float m, float q){
//        pos = p; vel = v; mass = m; neg_r = m; pos_r = q;
//    }
    mass_struct(glm::vec3 p, glm::vec3 v, float m, 
                float fpos_r, float fneg_r, float fpos_r2, float fneg_r2){
        pos = p; vel = v; mass = m; 
        pos_r = fpos_r; neg_r = fneg_r;
        pos_r2 = fpos_r2; neg_r2 = fneg_r2;
    }
    mass_struct(){
        pos = glm::vec3(0,0,0);vel = glm::vec3(0,0,0);
        mass = 0;
    }
};

class BallManager {
public:
    BallManager();
    ~BallManager();
    
    void init(const int n_masses);
    void destroy();
    
    void update();
    void updateMasses(float h);
    void draw(GameEngine::SpriteBatch& sb);
    void drawSprings(GameEngine::SpriteBatch& sb);
    
    void drawMasses(GameEngine::SpriteBatch& sb);
    void addMass(glm::vec3 pos, glm::vec3 vel, float m, 
                 float fpos_r, float fneg_r, float fpos_r2, float fneg_r2);
    void addSpring(int i, int j, float k, float l, float y);
    
    void setOutputFile(std::string filename)
        {m_outputFile = filename; m_writeOutput=true;}
    void writeOutputToFile();
    
    glm::vec3 getCM();
    
private:
    void correctCollisions();
    void calcStep(float t, float* src, float* dest);
    
    GLuint m_springtex;
    GLuint m_masstex;
    GameEngine::ColorRGBA8 m_masscolor;
    
    int m_width;
    int m_height;
    
    int m_size;
    int m_index = 0;
    
    mass_struct* m_masses;
    spring_struct** m_springs;
    
    bool m_hasCM = false;
    glm::vec3 m_cm;
    
    bool m_writeOutput = false;
    std::string m_outputFile;
    std::ofstream m_outputFileStream;
};

#endif	/* BALLMANAGER_H */

