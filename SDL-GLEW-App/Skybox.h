//
//  Skybox.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Skybox_h
#define Skybox_h

#include <GL/glew.h>

#include "Texture.h"
#include "GLContext.h"

#include <vector>


namespace Fox {
    
    class Skybox {
    
    public:
        Skybox(){
        
        }
        
        Skybox(std::vector<const GLchar*>& faces);
        
        GLuint loadCubeMap(std::vector<const GLchar*>& faces);
        
        void draw(GLContext* gl){
        
            glDepthFunc(GL_LEQUAL);
            gl->useShader(Shader::SKYBOX);
        
            gl->setProjectionUniform("projection");
            gl->setViewUniformForSkybox("view");

            // skybox cube
            glBindVertexArray(m_SkyboxVao);
            
            // bind cubemap texture to unit 0
            glActiveTexture(GL_TEXTURE0);
            gl->getCurrentShader()->setSampler("skybox", 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapId);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        
        }
        
        
        GLuint m_CubemapId; ///< cube map texture
        GLuint m_SkyboxVao; ///< skybox vertex array object
        GLuint m_SkyboxVbo; ///< skybox vertex buffer object
    
    };
}


#endif /* Skybox_h */
