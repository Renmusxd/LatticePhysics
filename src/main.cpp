#include <iostream>
#include <string>
#include "MainGame.h"

int main(int argc, char** argv){
    MainGame maingame;
    if (argc>1){
        std::cout << "Reading: " << argv[1] << '\n';
        if (argc>2){
            maingame.setOutputFile(std::string(argv[2]));
        }
        maingame.run(std::string(argv[1]));
    } else {
        std::cout << "Usage: physics <filename>\n";
    }
    return 0;
}