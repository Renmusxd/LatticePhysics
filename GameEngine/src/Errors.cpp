#include "Errors.h"
#include <iostream>
#include <cstdlib>
#include <SDL2/SDL.h>

//using namespace GameEngine;

void fatalError(std::string errorString){
	std::cout << errorString << std::endl;
	std::cout << "Enter any key to quit";
	int tmp;
	std::cin >> tmp;
	SDL_Quit();
	exit(1);
}