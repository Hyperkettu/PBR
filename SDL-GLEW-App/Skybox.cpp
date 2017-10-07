//
//  Skybox.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Skybox.h"


namespace Fox {
    
    Skybox::Skybox(std::vector<const GLchar*>& faces){
            
        m_CubemapId = loadCubeMap(faces);
            
        GLfloat skyboxVertices[] = {
                // Positions
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                
                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,
                
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                
                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,
                
                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,
                
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };
            
        glGenVertexArrays(1, &m_SkyboxVao);
        glGenBuffers(1, &m_SkyboxVbo);
        glBindVertexArray(m_SkyboxVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glBindVertexArray(0);
        
    }
    
    
    GLuint Skybox::loadCubeMap(std::vector<const GLchar*>& faces){
    
        // create new texture
        GLuint textureId;
        glGenTextures(1, &textureId);
        glActiveTexture(GL_TEXTURE0);
        
        // bind cube map texture
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
        
        std::vector<SDL_Surface*> images;
        
        // load all image faces of the cube map
        for(GLuint i = 0; i < faces.size(); i++){
        
            SDL_Surface* img = Texture::readTexture(faces[i]);
            
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img->w, img->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, img->pixels);
            
          //  SDL_FreeSurface(img);
            images.push_back(img);
            
            //Texture::freeTexture(img);
        }
        
        for(SDL_Surface* i : images){
            Texture::freeTexture(i);
        }
        
        // set parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        // unbind cube map
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        
        return textureId;
        
    }
}
