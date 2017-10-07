//
//  BatchRenderer.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "BatchRenderer.hpp"

#include <algorithm>

namespace Fox {
    
    BatchRenderer* BatchRenderer::m_Singleton = nullptr;
    
    void BatchRenderer::FlushSortedArray(Layer layer){
        
        Renderer& renderer = Application::GetRenderer();
        std::vector<GameObject*> objects = m_Layers[(unsigned int)layer];
        
     //   std::cout << layer << " " << objects.size() << std::endl;
        
        for(int i = 0; i < objects.size(); i++) {
        
            Mesh<VertexPNTTB>* mesh = objects[i]->GetComponent<MeshRenderer>()->m_Mesh;
            Transform* transform = objects[i]->GetComponent<Transform>();
        
            if(renderer.GetCurrentMaterial() != &mesh->m_Material){
                renderer.MaterialEnd();
                renderer.MaterialBegin(mesh->m_Material);
            }
            
            renderer.m_Shader->setMat4("model", transform->m_Model);
            mesh->draw(renderer.GetContext());
            
            renderer.MaterialEnd();
            
        }
        
    }
    
    void BatchRenderer::SortInAscendingOrder(Layer layer) {
        std::vector<GameObject*>& objects = m_Layers[layer];
        std::sort(objects.begin(), objects.end(), [](GameObject* a, GameObject* b) {
            
            Renderer& renderer = Application::GetRenderer();
            GLContext* gl = renderer.GetContext();
            
            MeshBase::Material& materialA = a->GetComponent<MeshRenderer>()->m_Mesh->m_Material;
            MeshBase::Material& materialB = b->GetComponent<MeshRenderer>()->m_Mesh->m_Material;
            
            glm::vec3 posA = a->GetComponent<Transform>()->m_Position;
            glm::vec3 posB = b->GetComponent<Transform>()->m_Position;
            glm::vec3 cameraPos = gl->getCurrentRenderContext().m_Camera.m_Position;
            
            float distanceA = glm::distance(posA, cameraPos);
            float distanceB = glm::distance(posB, cameraPos);
            
            return (distanceA < distanceB) ||
            ((distanceA == distanceB) && (&materialA < &materialB));
            
        });
    }
    
    void BatchRenderer::SortInDescendingOrder(Layer layer) {
        std::vector<GameObject*>& objects = m_Layers[layer];
        std::sort(objects.begin(), objects.end(), [](GameObject* a, GameObject* b) {
            
            Renderer& renderer = Application::GetRenderer();
            GLContext* gl = renderer.GetContext();
            
            MeshBase::Material& materialA = a->GetComponent<MeshRenderer>()->m_Mesh->m_Material;
            MeshBase::Material& materialB = b->GetComponent<MeshRenderer>()->m_Mesh->m_Material;
            
            glm::vec3 posA = a->GetComponent<Transform>()->m_Position;
            glm::vec3 posB = b->GetComponent<Transform>()->m_Position;
            glm::vec3 cameraPos = gl->getCurrentRenderContext().m_Camera.m_Position;
            
            float distanceA = glm::distance(posA, cameraPos);
            float distanceB = glm::distance(posB, cameraPos);
            
            return (distanceA > distanceB) ||
            ((distanceA == distanceB) && (&materialA < &materialB));
            
        });
    }
    
}
