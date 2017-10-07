//
//  ShadowMap.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 3/5/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef ShadowMap_h
#define ShadowMap_h

#include <GL/glew.h>

#include <iostream>

#include "GLContext.h"

namespace Fox {
    
    const GLuint SHADOW_WIDTH = 2*1024, SHADOW_HEIGHT = 2*1024;
    
    class ShadowMap {
    
    public:
        
        ShadowMap(){
            init();
        }
        
        void init();
        
        void renderToDepthBuffer(GLContext* gl);
        
        void switchToBackBuffer() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        
        void visualizeDepthBuffer(GLContext* gl){
            
         //   gl->useShader(5);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_DepthMap);
            
            glBindVertexArray(m_QuadVao);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
        
    private:
        
        GLuint m_DepthMapFBO; ///< depth map frame buffer object
        GLuint m_DepthMap; ///< depth map
        
        GLuint m_QuadVao; ///< quad for visualizing the depth map
        GLuint m_QuadVbo; ///< vertex buffer object for visualizing the depth map
    };
    
    
}

#endif /* ShadowMap_h */
