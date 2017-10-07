//
//  SceneGraph.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef SceneGraph_hpp
#define SceneGraph_hpp

#include "GameObject.hpp"
#include <stack>

namespace Fox {
    
    class UpdateClass
    {
    public:
        UpdateClass () {}
        void operator()(GameObject* obj);
    public:
        int deltaTime;
    };
    
    class SceneGraph {
    
    public:
        
        SceneGraph() {
            root = new GameObject;
            
            m_MatrixStack.push(glm::mat4(1.0f));
        }
        
        ~SceneGraph();
        
        void CollectAll(GameObject* parent, std::vector<GameObject*>& release) {
            
            release.push_back(parent);
            
            for(int i = 0; i < parent->m_Children.size(); i++){
                CollectAll(parent->m_Children[i], release);
            }
            
        }

        template <class Func>
        void TraverseGraph(Func func) { //void (*func)(GameObject*)) {
            TraverseChildren(root, func);
        }
        
        template<class Func>
        void TraverseChildren(GameObject* parent, Func func) {
            
            // apply function to all except root
            if(parent != root)
                func(parent);
            
            glm::mat4 mat = m_MatrixStack.top() * parent->GetComponent<Transform>()->m_Model;
            m_MatrixStack.push(mat);
        
            for(int i = 0; i < parent->m_Children.size(); i++){
                TraverseChildren(parent->m_Children[i], func);
            }
            
            m_MatrixStack.pop();
        }
    
        GameObject* root;
        UpdateClass update;
        
        static std::stack<glm::mat4> m_MatrixStack;
    };
}

#endif /* SceneGraph_hpp */
