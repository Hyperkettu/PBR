//
//  Renderer.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include "ShaderProgram.hpp"
#include "GLContext.h"
#include "ShaderManager.hpp"
#include "Mesh.h"
#include "Scene.hpp"
#include "CubeMapRenderer.hpp"
#include "PostProcess.hpp"

namespace Fox {

class Renderer {

public:
    
    Renderer() {
    }
    
    ~Renderer() {
    }
    
    void PreCompute();
    
    void Begin(GLContext* gl){
        m_Context = gl;
    }
    
    void End() {
        m_Context = nullptr;
    }
    
    void MaterialBegin(MeshBase::Material& material);
    
    void MaterialEnd();
    
    inline MeshBase::Material* GetCurrentMaterial(){
        return m_Material;
    }
    
    
    void ResolveVisibility(Scene& scene);
    
    
    inline ShaderManager& GetShaderManager(){
        return m_ShaderManager;
    }
    
    inline GLContext* GetContext(){
        return m_Context;
    }
    
    void Render(Scene& scene);
    
    inline Scene* GetScene() {
        return m_Scene;
    }
    
    void SetSkyboxToCubemapRenderer(Skybox& skybox){
        m_CubeMapRenderer.SetSkyBox(skybox);
    }
    
    inline PostProcess& GetPostProcess(){
        return m_postProcess;
    }
    
    void FillScreenWithTexture(){
        m_postProcess.RenderToScreen(m_CubeMapRenderer.m_Brdf);
    }
    
    inline Texture* GetBRDFIntegrationMap() {
        return m_CubeMapRenderer.GetBRDFIntegrationMap();
    }
    
    inline Texture* GetIrradianceMap() {
        return m_CubeMapRenderer.GetIrradianceMap();
    }
    
    inline Texture* GetPrefilterMap() {
        return m_CubeMapRenderer.GetPrefilterMap();
    }
    
    ShaderProgram* m_Shader;
    
private:
    
    GLContext* m_Context;
    ShaderManager m_ShaderManager;
    
    MeshBase::Material* m_Material;
    Scene* m_Scene;
    
    CubeMapRenderer m_CubeMapRenderer;
    PostProcess m_postProcess;
};
    
}

#endif /* Renderer_hpp */
