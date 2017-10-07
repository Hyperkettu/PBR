//
//  ResourceManager.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef ResourceManager_h
#define ResourceManager_h

#include <iostream>
#include <fstream>
#include <cstring>

#include <GL/glew.h>

#include "TextureManager.h"
#include "Texture.h"

namespace Fox {

class ResourceManager {

public:
    
    ResourceManager(){
        
    }
    
    /**
     * Loads file to memory block
     * @param fname name of the file to be loaded
     * @param fSize file size bytes
     * @return memory block
     */
    static char* loadFile(char *fname, GLint &fSize);
    
};
    
}


#endif /* ResourceManager_h */
