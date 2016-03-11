/* 
 * File:   ObjectLoader.h
 * Author: Sumner
 *
 * Created on March 11, 2016, 1:49 PM
 */

#ifndef OBJECTLOADER_H
#define	OBJECTLOADER_H

#include <string>
#include "BallManager.h"

class ObjectLoader {
public:
    ObjectLoader();
    ObjectLoader(const ObjectLoader& orig);
    virtual ~ObjectLoader();
    
    bool initializeBallManagerFromFile(BallManager* bm, std::string filename);
    
private:

};

#endif	/* OBJECTLOADER_H */

