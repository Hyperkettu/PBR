//
//  Scene.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include "SceneGraph.hpp"
#include "GeometryGenerator.h"
#include "Skybox.h"
#include "Light.h"

namespace Fox {

    
    class Scene {
    
    public:
        
        Scene() {
            m_SunAngle = 0.0f;
        }
        
        void InitScene();
        
        
        void AddObject(GameObject* obj){
            sceneGraph.root->AddChild(obj);
        }
        
        inline SceneGraph& GetSceneGraph() {
            return sceneGraph;
        }
        
        inline Skybox& GetSkyBox(){
            return m_Skybox;
        }
        
        inline DirectionalLight& GetSun() {
            return m_Sun;
        }
        
        void Update();
        
    private:
        
        float m_SunAngle;
        
        SceneGraph sceneGraph;
        Skybox m_Skybox;
        DirectionalLight m_Sun;
    
    };
}

#endif /* Scene_hpp */
