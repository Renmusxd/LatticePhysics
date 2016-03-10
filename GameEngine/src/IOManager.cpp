#include "IOManager.h"
#include <fstream>

using namespace GameEngine;

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer){
	// read in binary
	std::ifstream file(filePath,std::ios::binary);
	if (file.fail()){
		perror(filePath.c_str());
		return false;
	}
	// Find size of file
		// seek to end
	file.seekg(0,std::ios::end);
		// get filesize in bytes
	int fileSize = file.tellg();
		// go back to start
	file.seekg(0,std::ios::beg);

	fileSize -= file.tellg(); // in case header is included

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]),fileSize); // fill starting from address of [0]

	file.close();

	return true;
}

bool IOManager::readFileToBuffer(std::string filePath, std::string& buffer){
	// read in binary
	std::ifstream file(filePath,std::ios::binary);
	if (file.fail()){
		perror(filePath.c_str());
		return false;
	}
	// Find size of file
		// seek to end
	file.seekg(0,std::ios::end);
		// get filesize in bytes
	int fileSize = file.tellg();
		// go back to start
	file.seekg(0,std::ios::beg);

	fileSize -= file.tellg(); // in case header is included

	buffer.resize(fileSize);
	file.read((char*)&(buffer[0]),fileSize); // fill starting from address of [0]

	file.close();

	return true;
}