//
//  ConstantBuffers.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef ConstantBuffers_h
#define ConstantBuffers_h

#include "Application.h"

namespace Fox {
    
    class ConstantBuffers {
    
    public:
        
        ConstantBuffers() {}
        
        static void Sync() {
            
            Renderer& renderer = Application::GetRenderer();
            GLContext* gl = renderer.GetContext();
            
            ShaderProgram* shader = gl->getCurrentShader();
            
            // sync camera tranforms
            gl->setCameraPosition("viewPos");
            gl->setViewUniform("view");
            gl->setProjectionUniform("projection");
            
            shader->setFloat("dispFactor", 1.5f);
            
            // set pbr material properties
       /*     shader->setVec3("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
            shader->setFloat("ao", 1.0f);
            shader->setFloat("metallic", 0.01f);
            shader->setFloat("roughness", 0.0f);
         */   
            gl->SetSamplerTexture(renderer.GetIrradianceMap(), "irradianceMap", 6);
            gl->SetSamplerTexture(renderer.GetPrefilterMap(), "prefilterMap", 7);
            gl->SetSamplerTexture(renderer.GetBRDFIntegrationMap(), "brdfLUT", 8);
            
            shader->setVec3("sun.color", renderer.GetScene()->GetSun().m_Color);
            shader->setVec3("sun.direction", renderer.GetScene()->GetSun().m_Direction);
            
         /*   // sync directional light (color and direction)
            shader->setVec3("dirLight.ambient", renderer.GetScene()->GetSun().m_Ambient);
            shader->setVec3("dirLight.diffuse", renderer.GetScene()->GetSun().m_Diffuse);
            shader->setVec3("dirLight.specular", renderer.GetScene()->GetSun().m_Specular);
            shader->setVec3("dirLight.direction", renderer.GetScene()->GetSun().m_Direction);
           */
            // sync spot light
        /*    shader->setVec3("spotLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
            shader->setVec3("spotLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
            shader->setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader->setFloat("spotLight.constant", 1.0f);
            shader->setFloat("spotLight.linear", 0.09f);
            shader->setFloat("spotLight.quadratic", 0.032f);
            shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
            shader->setVec3("spotLight.position", gl->getCurrentRenderContext().m_Camera.m_Position);
            shader->setVec3("spotLight.direction", gl->getCurrentRenderContext().m_Camera.m_Front); */
        
        }
        
    private:
    
    };
}


#endif /* ConstantBuffers_h */
