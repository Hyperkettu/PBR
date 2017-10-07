//
//  ShadowMap.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 3/5/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

//#include "Application.h"
#include "ShadowMap.h"

namespace Fox {
    
    void ShadowMap::init(){
            
        // generate frame buffer object
        glGenFramebuffers(1, &m_DepthMapFBO);
        // generate depth map texture
        glGenTextures(1, &m_DepthMap);
        glBindTexture(GL_TEXTURE_2D, m_DepthMap);
        // create depth map
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        
        // set parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
        // attach depth texture as framebuffers depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // setup quad for visualizing the depth buffer
        GLfloat quadVertices[] = {
            // Positions        // Texture Coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        
        // Setup plane VAO
        glGenVertexArrays(1, &m_QuadVao);
        glGenBuffers(1, &m_QuadVbo);
        glBindVertexArray(m_QuadVao);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    
    void ShadowMap::renderToDepthBuffer(GLContext* gl){
    
        
        RenderContext& rc = gl->getCurrentRenderContext();
        
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = rc.m_Near, far_plane = 1000.0f;//rc.m_Far;
        //lightProjection = glm::ortho(-600.0f, 700.0f, -200.0f, 400.0f, near_plane, far_plane);
        lightProjection = glm::ortho(-400.0f, 400.0f, -400.0f, 400.f, near_plane, far_plane);
        lightView = glm::lookAt(glm::vec3(-2.0f, 3.0f, -3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
       // lightView = glm::lookAt(glm::vec3(-2.0, 4.0f, -1.0f), glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        
        // render scene from light's point of view
     //   gl->useShader(4);
        gl->getCurrentShader()->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        // change viewport to match the shadow map
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        
        // switch to depth map buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);        
    
    }
}
