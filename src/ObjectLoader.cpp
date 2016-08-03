/* 
 * File:   ObjectLoader.cpp
 * Author: Sumner
 * 
 * Created on March 11, 2016, 1:49 PM
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "ObjectLoader.h"

ObjectLoader::ObjectLoader() {
}

ObjectLoader::~ObjectLoader() {
}

struct frame_struct{
    glm::vec3 pos;
    glm::vec3 vel;
    frame_struct(glm::vec3 p, glm::vec3 v){
        pos = p; vel = v;
    }
};

bool ObjectLoader::initializeBallManagerFromFile(BallManager* bm, std::string filename){
    /* Loads a file into a ball manager */
    std::string line;
    std::ifstream f(filename);
    if (f.is_open()){
        int n_masses = 0;
        if (getline(f,line)){
            // Read number of masses
            std::istringstream ss(line);
            ss >> n_masses;
        }
        bm->init(n_masses);
        
        
        while ( getline(f,line) )
        {
            std::string cmd;
            float v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 = 0;
            int indx1, indx2 = 0;
            std::istringstream ss(line);
            ss >> cmd;
            frame_struct frame(glm::vec3(0,0,0),glm::vec3(0,0,0));
            if (strncmp(cmd.c_str(),"FRAME",5)==0){
                ss >> v1 >> v2 >> v3 >> v4 >> v5 >> v6;
                frame.pos = glm::vec3(v1,v2,v3);
                frame.vel = glm::vec3(v4,v5,v6);
            } else if (strncmp(cmd.c_str(),"MASS",4)==0){
                ss >> v1 >> v2 >> v3 // pos
                   >> v4 >> v5 >> v6 // vel
                   >> v7             // m
                   >> v8 >> v9       // r
                   >> v10 >> v11;    // r2
                if (n_masses==0){
                    // ERROR! TOO MANY MASSES!
                }
                bm->addMass(frame.pos + glm::vec3(v1,v2,v3),
                            frame.vel + glm::vec3(v4,v5,v6),
                            v7,
                            v8,v9,
                            v10,v11);
            } else if (strncmp(cmd.c_str(),"SPRING",6)==0){
                ss >> indx1 >> indx2 >> v1 >> v2 >> v3;
                bm->addSpring(indx1,indx2,v2,v1,v3);
            }
        }
        return true;
    } else {
        bm->init(0);
        return false;
    }
}