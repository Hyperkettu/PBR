//
//  GameObject.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Mesh.h"
//#include "Model.h"

namespace Fox {
    
    class Component {
        
    public:
        
        Component() {}
        
         virtual void GetComponent() {}
        
    };
    
    class MeshRenderer : public Component {
        
    public:
        
        enum Layer {
            OPAQUE = 0,
            TRANSPARENT
        };
        
        MeshRenderer() {}
        
        void GetComponent() {}
        
        Mesh<VertexPNTTB>* m_Mesh;
        Layer m_Layer;
        
    };
    
    class Transform : public Component {
        
    public:
        
        Transform() {
        
            m_Position = glm::vec3(0.0f);
            m_EulerAngles = glm::vec3(0.0f);
            m_Scale = glm::vec3(1.0f);
            m_Rotation = glm::toQuat(glm::mat3(1.0f));
            
            glm::mat4 identity = glm::mat4(1.0f);
            ComposeModelTransform(identity);
        }
        
        void ComposeModelTransform(glm::mat4& parent) {
        
         /*   m_Model = glm::mat4();
            m_Model = glm::translate(m_Model, m_Position);
            m_Model = glm::rotate(m_Model, m_EulerAngles.z, glm::vec3(0.0f, 1.0f, 0.0f));
            m_Model = glm::rotate(m_Model, m_EulerAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
            m_Model = glm::rotate(m_Model, m_EulerAngles.x, glm::vec3(0.0f, 1.0f, 0.0f));
            m_Model = glm::scale(m_Model, m_Scale);
           */
            glm::mat4 trans = glm::translate(glm::mat4(1.0f), m_Position);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
            m_Model = glm::toMat4(m_Rotation);
            m_Model = trans*m_Model*scale;
            m_Model = parent * m_Model;
        }
        
        void Rotate(glm::vec3 eulerAngles) {
            m_Rotation = glm::quat(eulerAngles);
        }
        
        void GetComponent() {}
        
        glm::vec3 m_Position;
        glm::vec3 m_EulerAngles;
        glm::vec3 m_Scale;
        
        glm::quat m_Rotation;
        
        glm::mat4 m_Model;
        
    };
   
    class GameObject {
    
    public:
        
        GameObject() {
        
            AddComponent(new Transform);
            AddComponent(new MeshRenderer());
        
        }
        
        void Update();
        
        GameObject* Find(std::string& name);
        
        GameObject* TraverseChildren(GameObject* parent, std::string name) {
            
            if(parent != nullptr && parent->m_Name == name)
                return parent;
            
            for(int i = 0; i < parent->m_Children.size(); i++){
                return TraverseChildren(parent->m_Children[i], name);
            }
            
            return nullptr;
        }
        
        void AddComponent(Component* component){
            m_Components.push_back(component);
        }
        
        void AddChild(GameObject* child){
            m_Children.push_back((child));
        }
        
        template<class Comp>
        Comp* GetComponent(){
            
            for(int i = 0; i < m_Components.size(); i++) {
                
                Comp* c = dynamic_cast<Comp*>(m_Components[i]);
                
                if(c != nullptr) {
                    return c;
                }
            }
            
            return nullptr;
        }
        
        std::vector<GameObject*> m_Children;
        std::string m_Name;
        
    private:
        
        std::vector<Component*> m_Components;
    
    };
}

#endif /* GameObject_hpp */
