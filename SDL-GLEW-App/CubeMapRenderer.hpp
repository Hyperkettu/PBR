//
//  CubeMapRenderer.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/20/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef CubeMapRenderer_hpp
#define CubeMapRenderer_hpp

#include <GL/glew.h>

#include "Skybox.h"

namespace Fox {
    
    class CubeMapRenderer {
    
    public:
        
        CubeMapRenderer() {
            m_QuadVAO = 0;
            
         //   m_BRDFIntegrationMap = new Texture;
            m_PrefilterMap = new Texture;
            m_IrradianceMap = new Texture;

        }
        ~CubeMapRenderer(){}
        
        void GenerateMips();
        
        void GenerateIrradianceMap();
        
        void GeneratePrefilteredEnvMap();
        
        void GenerateIntegrationBRDFMap();
    
    
        void RenderCube();
        
        void RenderQuad();
        
        
        void SetSkyBox(Skybox& skybox){
            m_Skybox = &skybox;
        }
        
        
        inline Texture* GetBRDFIntegrationMap(){
            return m_BRDFIntegrationMap;
        }
        
        inline Texture* GetPrefilterMap(){
            return m_PrefilterMap;
        }
        
        inline Texture* GetIrradianceMap(){
            return m_IrradianceMap;
        }
        
        GLuint m_Brdf;
        
    private:
        
        Texture* m_BRDFIntegrationMap;
        Texture* m_PrefilterMap;
        Texture* m_IrradianceMap;
    
        GLuint m_CubeVAO, m_CubeVBO;
        GLuint m_QuadVAO;
        GLuint m_QuadVBO;
        
        Skybox* m_Skybox;
        
    };
}

#endif /* CubeMapRenderer_hpp */
