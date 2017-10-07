//
//  SceneGraph.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "SceneGraph.hpp"

namespace Fox {
    
    std::stack<glm::mat4> SceneGraph::m_MatrixStack;
    
    void UpdateClass::operator()(GameObject* obj) {
        obj->Update();
    }
    
    SceneGraph::~SceneGraph() {
        
        std::vector<GameObject*> release;
        
        CollectAll(root, release);
        
        for(GameObject* obj : release)
            delete obj;
        
    }
}
