//
//  Framebuffer.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include <GL/glew.h>

#include "Texture.h"

namespace Fox {
    
    class Framebuffer {
    
    public:
        
        Framebuffer() {}
        
        
        void Create() {
            glGenFramebuffers(1, &m_Id);
            glGenRenderbuffers(1, &m_RBO);
        }
        
        void Bind(){
            glBindFramebuffer(GL_FRAMEBUFFER, m_Id);
            glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        }
        
        void AttachTexture(Texture* texture, GLuint colorRenderTargetIndex){
            Bind();
            glBindTexture(GL_TEXTURE_2D, texture->m_Id);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorRenderTargetIndex, texture->m_Id, 0);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, texture->m_Width, texture->m_Height);
        }
        
        void Destroy() {
        
            glDeleteFramebuffers(1, &m_Id);
            glDeleteRenderbuffers(1, &m_RBO);
        
        }
        
        GLuint m_Id;
        GLuint m_RBO;
    
    };
}

#endif /* Framebuffer_hpp */
