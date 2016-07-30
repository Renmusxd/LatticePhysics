#include <iostream>
#include <string>
#include <stdio.h>
#include "MainGame.h"

int main(int argc, char** argv){    
    std::string filename;
    std::string outputfile;
    int simlength = -1;
    bool usegui = false;
    for (int i = 1; i<argc; i++){
        std::string arg = std::string(argv[i]);
        
        if (arg[0] == '-'){
            std::string gui("-GUI");
            if (gui.compare(arg)==0){
                usegui = true;
            }
        } 
        else if (filename.empty()){
            filename = std::string(arg);
        } 
        else if (outputfile.empty()) {
            outputfile = std::string(arg);
        } else {
            simlength = std::stoi(std::string(arg));
        }
    }
    
    if (!filename.empty()){
        if (usegui){
            printf("In GUI");
            MainGame maingame;
            if (!outputfile.empty()){
                maingame.setOutputFile(outputfile);
            }
            maingame.run(filename,simlength);
        } else {
            BallManager ballman;
            if (!outputfile.empty()){
                ballman.setOutputFile(outputfile);
            }
            for (;simlength!=0; simlength--){
                ballman.update();
            }
        }
    } else {
        std::cout << "Usage: physics [options] <input> [output] [timesteps]\n";
        return 1;
    }
    return 0;
}