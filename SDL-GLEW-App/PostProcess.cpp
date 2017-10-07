//
//  PostProcess.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/20/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "PostProcess.hpp"
#include "Application.h"

namespace Fox {
    
    void PostProcess::RenderQuad() {
        
        if (m_QuadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &m_QuadVAO);
            glGenBuffers(1, &m_QuadVBO);
            glBindVertexArray(m_QuadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(m_QuadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
    
    void PostProcess::RenderToScreen(GLuint texture){
        
        GLContext* gl = Application::GetRenderer().GetContext();
        
        gl->useShader(Shader::FILL_SCREEN);
        gl->bindTexture(texture, GL_TEXTURE0, "screenTexture", 0);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        RenderQuad();
    }
}
