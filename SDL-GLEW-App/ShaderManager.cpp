//
//  ShaderManager.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "ShaderManager.hpp"

namespace Fox {

    std::vector<ShaderProgram*> ShaderManager::m_Shaders = std::vector<ShaderProgram*>(Shader::NUM_SHADERS);
    
    void ShaderManager::AddShaderProgram(char* vs, char* tcs, char* tes, char* fs, Shader type) {
        
        GLint vertexShaderFileSize;
        char* vertexShaderData = ResourceManager::loadFile(vs, vertexShaderFileSize);
        
        
        GLint tcsFileSize;
        char* tcsData = ResourceManager::loadFile(tcs, tcsFileSize);
        
        GLint tesFileSize;
        char* tesData = ResourceManager::loadFile(tes, tesFileSize);
        
        GLint fragmentShaderFileSize;
        char* fragmentShaderData = ResourceManager::loadFile(fs, fragmentShaderFileSize);
        
        m_Shaders[type] = new ShaderProgram(vertexShaderData, vertexShaderFileSize, tcsData, tcsFileSize, tesData, tesFileSize, fragmentShaderData, fragmentShaderFileSize);
        
    }

}
