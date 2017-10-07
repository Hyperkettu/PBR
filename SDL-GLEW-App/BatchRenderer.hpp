//
//  BatchRenderer.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef BatchRenderer_hpp
#define BatchRenderer_hpp

#include <vector>

#include "Application.h"
#include "Renderer.hpp"
#include "GameObject.hpp"
#include "Mesh.h"

namespace Fox {
    
    class BatchRenderer {
        
    public:
        
        enum Layer {
        
            OPAQUE = 0,
            TRANSPARENT,
            
            NUM_LAYERS
        };
        
        BatchRenderer() {
        
            m_Layers = std::vector<std::vector<GameObject*>>(NUM_LAYERS);
        
        }
        
        ~BatchRenderer(){
            delete BatchRenderer::Instance();
        }
        
        void SortInAscendingOrder(Layer layer);
        
        void SortInDescendingOrder(Layer layer);
        
        void FlushSortedArray(Layer layer);
        
        /**
         * Clears all layers
         */
        void Reset() {
        
            for(int i = 0; i < NUM_LAYERS; i++){
                m_Layers[i].clear();
            }
        }
        
        static BatchRenderer* Instance() {
            
            if (m_Singleton == nullptr)
                m_Singleton = new BatchRenderer;
            
            return m_Singleton;
        }
        
        void AddObject(GameObject* obj, Layer layer){
            m_Layers[layer].push_back(obj);
        }
        
    private:
        
        static BatchRenderer* m_Singleton;
        
        std::vector<std::vector<GameObject*>> m_Layers;
    
    };
    
}

#endif /* BatchRenderer_hpp */
