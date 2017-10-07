//
//  CubeMapRenderer.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/20/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "CubeMapRenderer.hpp"

#include "Application.h"
#include "cubeData.h"

namespace Fox {
    
    static int IRRADIANCE_CUBEMAP_SIZE = 64;
    static int PREFILTERED_ENV_CUBEMAP_SIZE = 512;
    static int INTEGRATION_BRDF_MAP_SIZE = 512;

    void CubeMapRenderer::GenerateMips() {
        
    
    }
    
    void CubeMapRenderer::GenerateIrradianceMap() {
        
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); 
        
        Renderer& renderer = Application::GetRenderer();
        GLContext* gl = renderer.GetContext();
       
        // create irradiance cube map
        GLuint irradianceMap;
        glGenTextures(1, &irradianceMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IRRADIANCE_CUBEMAP_SIZE, IRRADIANCE_CUBEMAP_SIZE, 0,
                         GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
        // create corresponding framebuffer
        GLuint captureFBO, captureRBO;
        
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);
        
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        
        
        gl->useShader(Shader::IRRADIANCE);
        ShaderProgram* irradianceShader = gl->getCurrentShader();
        
        irradianceShader->setSampler("environmentMap", 0);
        // 90 degrees perspective projection
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f); // 10.0f initially
        irradianceShader->setMat4("projection", captureProjection);
        
        // bind environment source
        glActiveTexture(GL_TEXTURE0);
        
        // use skybox as convolution source
        GLuint envCubeMap = m_Skybox->m_CubemapId;
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubeMap);
        
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
        
        glViewport(0, 0, IRRADIANCE_CUBEMAP_SIZE, IRRADIANCE_CUBEMAP_SIZE); // don't forget to configure the viewport to the capture dimensions.
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        for (unsigned int i = 0; i < 6; ++i)
        {
            irradianceShader->setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            RenderCube();
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);
    
        // finally set convoluted skybox as skybox
    //   m_Skybox->m_CubemapId = irradianceMap;
        
        m_IrradianceMap->m_Id = irradianceMap;
        m_IrradianceMap->m_IsCubeMap = true;
    }
    
    
    void CubeMapRenderer::GeneratePrefilteredEnvMap() {
    
        Renderer& renderer = Application::GetRenderer();
        GLContext* gl = renderer.GetContext();
        
        // create prefiltered env map cube map
        GLuint prefilterMap;
        glGenTextures(1, &prefilterMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        
        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, PREFILTERED_ENV_CUBEMAP_SIZE, PREFILTERED_ENV_CUBEMAP_SIZE, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // linear mip map filtering to enable roughness values smoothly between 0 .. 0.2 .. 0.4 ... 1.0
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // allocate memory also for mip maps
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        
        
        // create corresponding framebuffer
        GLuint captureFBO, captureRBO;
        
        glGenFramebuffers(1, &captureFBO);
        glGenRenderbuffers(1, &captureRBO);
        
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
        
        gl->useShader(Shader::PREFILTERED_ENV_MAP);
        ShaderProgram* prefilterShader = gl->getCurrentShader();
        prefilterShader->setSampler("environmentMap", 0);
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f); // 10.0f initially
        prefilterShader->setMat4("projection", captureProjection);
        
        // bind skybox as source cubemap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox->m_CubemapId);
        
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
        
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        GLuint maxMipLevels = 5;
        
        for (GLuint mip = 0; mip < maxMipLevels; ++mip)
        {
            // resize framebuffer according to mip-level size.
            GLuint mipWidth  = PREFILTERED_ENV_CUBEMAP_SIZE * std::pow(0.5, mip);
            GLuint mipHeight = PREFILTERED_ENV_CUBEMAP_SIZE * std::pow(0.5, mip);
            glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
            
            glViewport(0, 0, mipWidth, mipHeight);
            
            float roughness = (float) mip / (float)(maxMipLevels - 1);
            prefilterShader->setFloat("roughness", roughness);
            
            for (unsigned int i = 0; i < 6; ++i)
            {
                prefilterShader->setMat4("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);
                
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                RenderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        
        glDeleteFramebuffers(1, &captureFBO);
        glDeleteRenderbuffers(1, &captureRBO);
        
        m_PrefilterMap->m_Id = prefilterMap;
        m_PrefilterMap->m_IsCubeMap = true;
        
      //  m_Skybox->m_CubemapId = prefilterMap;
        
    }
    
    void CubeMapRenderer::GenerateIntegrationBRDFMap() {
    
        Renderer& renderer = Application::GetRenderer();
        GLContext* gl = renderer.GetContext();
        
        m_BRDFIntegrationMap = Texture::CreateRenderTarget(GL_RG16F, GL_RG, INTEGRATION_BRDF_MAP_SIZE, INTEGRATION_BRDF_MAP_SIZE);
        RenderTargetState rts(0, 0, INTEGRATION_BRDF_MAP_SIZE, INTEGRATION_BRDF_MAP_SIZE);
        rts.AddColorTarget(0, m_BRDFIntegrationMap);
        gl->RenderTargetBegin(rts);
      
        gl->useShader(Shader::INTEGRATION_BRDF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderQuad();
        
        gl->RenderTargetEnd();
        
        m_Brdf = m_BRDFIntegrationMap->m_Id;
    }
    
    void CubeMapRenderer::RenderCube() {
    
        // initialize (if necessary)
        if (m_CubeVAO == 0)
        {
            glGenVertexArrays(1, &m_CubeVAO);
            glGenBuffers(1, &m_CubeVBO);
            // fill buffer
            glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);
            // link vertex attributes
            glBindVertexArray(m_CubeVAO);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
        // render Cube
        glBindVertexArray(m_CubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    
    void CubeMapRenderer::RenderQuad() {
        
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
}
