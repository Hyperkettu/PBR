//
//  RenderTargetState.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef RenderTargetState_hpp
#define RenderTargetState_hpp

#include <GL/glew.h>

#include <stack>

#include "Framebuffer.hpp"

namespace Fox {
    
    class RenderTargetState {
        
        static const GLint NUM_RENDER_TARGETS = 4;
    
    public:
        
        RenderTargetState(GLint viewPortX, GLint viewPortY, GLint viewPortWidth, GLint viewPortHeight) :
            m_ViewPortX(viewPortX), m_ViewPortY(viewPortY), m_ViewPortWidth(viewPortWidth), m_ViewPortHeight(viewPortHeight)
        {
            m_Framebuffer.Create();
        }
        
        void AddColorTarget(GLuint index, Texture* texture) {
            m_Framebuffer.AttachTexture(texture, index);
        }
        
        void AddDepthStencilTarget() {}
    
        void Destroy() {
            m_Framebuffer.Destroy();
        }
        
        void SetAsCurrentRenderTarget(){
            m_Framebuffer.Bind();
        }
    
        static std::stack<RenderTargetState> renderTargetStack;
        
        
        
        Framebuffer m_Framebuffer;
        GLint m_ViewPortX, m_ViewPortY, m_ViewPortWidth, m_ViewPortHeight;
    };
}

#endif /* RenderTargetState_hpp */
