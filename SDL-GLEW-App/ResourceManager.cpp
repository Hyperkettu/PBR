//
//  ResourceManager.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "ResourceManager.h"

namespace Fox {

char* ResourceManager::loadFile(char* fname, int &fSize) {
    
    std::ifstream::pos_type size;
    char * memblock;
    std::string text;
    
    // file read based on example in cplusplus.com tutorial
    std::ifstream file (fname, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open()) {
        size = file.tellg();
        fSize = (GLuint) size;
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        
        text.assign(memblock);
    }
    else {
        std::cout << "Unable to open file " << fname << std::endl;
        exit(1);
    }
    
    return memblock;
    
}
    
}
