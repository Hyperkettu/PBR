//
//  Renderer.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Renderer.hpp"
#include "BatchRenderer.hpp"
#include "ConstantBuffers.hpp"

namespace Fox {


    void Renderer::Render(Scene& scene) {
        
        m_Scene = &scene;
        
        glEnable(GL_DEPTH_TEST);
        
        ResolveVisibility(scene);
        
        BatchRenderer* batchRenderer = BatchRenderer::Instance();
        
        batchRenderer->SortInAscendingOrder(BatchRenderer::OPAQUE);
        batchRenderer->SortInDescendingOrder(BatchRenderer::TRANSPARENT);
        
        // draw layers
        batchRenderer->FlushSortedArray(BatchRenderer::OPAQUE);
        batchRenderer->FlushSortedArray(BatchRenderer::TRANSPARENT);
        
        // synchronize shader contants
        ConstantBuffers::Sync();
        
        // draw skybox last
        scene.GetSkyBox().draw(m_Context);
        
    }
    
    void Renderer::MaterialBegin(MeshBase::Material& material) {
        
        m_Material = &material;
        m_Shader = ShaderManager::GetShader(material.m_Shader);
        
        m_Context->useShader(material.m_Shader);
        
        // bind all existing textures
        for(GLuint i = 0; i < material.m_Textures.size(); i++){
            
            Texture* texture = material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                m_Context->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
            
        }
        
        glActiveTexture(GL_TEXTURE0);
        
    }
    
    void Renderer::MaterialEnd(){
        m_Shader = nullptr;
        
        if(m_Material == nullptr)
            return;
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material->m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        m_Material = nullptr;
    }
    
    void FrustumCulling(GameObject* graphNode){
        
        Renderer& renderer = Application::GetRenderer();
        GLContext* gl = renderer.GetContext();
        
        Transform* transform = graphNode->GetComponent<Transform>();
        MeshRenderer* mesh = graphNode->GetComponent<MeshRenderer>();
        
        if(mesh->m_Mesh == nullptr) {
            return;
        }
        
        gl->getCurrentRenderContext().updateFrustum(transform->m_Model);
        
        if(gl->getCurrentRenderContext().m_Frustum.sphereIsInsideFrustum(
           mesh->m_Mesh->m_BoundingSphere.m_Center, mesh->m_Mesh->m_BoundingSphere.m_Radius))
            BatchRenderer::Instance()->AddObject(graphNode,
                                             (BatchRenderer::Layer)graphNode->GetComponent<MeshRenderer>()->m_Layer);
        
    }
    
    void Renderer::ResolveVisibility(Scene& scene) {
        
        // currently just pass all objects through without frustum culling
        BatchRenderer::Instance()->Reset();
        scene.GetSceneGraph().TraverseGraph(FrustumCulling);

        
    
    }
    
    void Renderer::PreCompute() {
        
        m_CubeMapRenderer.GenerateIrradianceMap();
        m_CubeMapRenderer.GeneratePrefilteredEnvMap();
        m_CubeMapRenderer.GenerateIntegrationBRDFMap();
    }

}
