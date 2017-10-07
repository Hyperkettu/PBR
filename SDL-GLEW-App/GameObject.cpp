//
//  GameObject.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "GameObject.hpp"
#include "Time.h"
#include "SceneGraph.hpp"

namespace Fox {
    
    void GameObject::Update() {
        
     //   if(m_Name == "nanosuit.obj"){

            Transform* transform = GetComponent<Transform>();
            transform->m_EulerAngles.y += 1.0f * Time::deltaTime;
            transform->Rotate(transform->m_EulerAngles);
     //   }
    
        GetComponent<Transform>()->ComposeModelTransform(SceneGraph::m_MatrixStack.top());
    }
    
    GameObject* GameObject::Find(std::string& name) {
        return TraverseChildren(this, name);
    }
    
}
