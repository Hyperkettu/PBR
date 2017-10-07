//
//  ShaderManager.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef ShaderManager_hpp
#define ShaderManager_hpp

#include <vector>
#include <string>

#include "ShaderProgram.hpp"
#include "ResourceManager.h"

namespace Fox {

class ShaderManager {

public:
    
    ShaderManager() {
    
        
    }
    
    ~ShaderManager(){

        for(ShaderProgram* shader : m_Shaders)
            delete shader;
    }
    
    void LoadShaders(){
        AddShaderProgram("Shaders/bumpedDiffuseSpecular.vert", "Shaders/bumpedDiffuseSpecular.frag", Shader::BLINN_PHONG);
        AddShaderProgram("Shaders/skybox.vert", "Shaders/skybox.frag", Shader::SKYBOX);
        AddShaderProgram("Shaders/irradiance.vert", "Shaders/irradiance.frag", Shader::IRRADIANCE);
        AddShaderProgram("Shaders/irradiance.vert", "Shaders/prefilter.frag", Shader::PREFILTERED_ENV_MAP);
        AddShaderProgram("Shaders/brdf.vert", "Shaders/brdf.frag", Shader::INTEGRATION_BRDF);
        AddShaderProgram("Shaders/brdf.vert", "Shaders/drawToScreen.frag", Shader::FILL_SCREEN);
        AddShaderProgram("Shaders/pbr.vert", "Shaders/pbr.frag", Shader::PBR);
        AddShaderProgram("Shaders/pbrWithDisplacement.vert", "Shaders/pbr.tesc", "Shaders/pbr.tese", "Shaders/pbrWithDisplacement.frag", Shader::PBR_DISPLACEMENT);
    }
    
    void AddShaderProgram(char* vertexShaderFile, char* fragmentShaderFile, Shader type) {
    
        GLint vertexShaderFileSize;
        char* vertexShaderData = ResourceManager::loadFile(vertexShaderFile, vertexShaderFileSize);
        
        GLint fragmentShaderFileSize;
        char* fragmentShaderData = ResourceManager::loadFile(fragmentShaderFile, fragmentShaderFileSize);
        
        m_Shaders[type] = new ShaderProgram(vertexShaderData, vertexShaderFileSize, fragmentShaderData, fragmentShaderFileSize);
    
    }
    
    void AddShaderProgram(char* vs, char* tcs, char* tes, char* fs, Shader type);
    
    inline static ShaderProgram* GetShader(Shader type){
        return m_Shaders[type];
    }
    
    
private:
    
    static std::vector<ShaderProgram*> m_Shaders;

};
    
}

#endif /* ShaderManager_hpp */
