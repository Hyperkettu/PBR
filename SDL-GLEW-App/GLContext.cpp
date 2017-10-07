//
//  GLContext.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "GLContext.h"

namespace Fox {
    
    void GLContext::RenderTargetBegin(RenderTargetState& rts) {
        
        RenderTargetState::renderTargetStack.push(rts);
        
        // set render targets (max 4)
        rts.SetAsCurrentRenderTarget();
        
        // set view port
        glViewport(rts.m_ViewPortX, rts.m_ViewPortY, rts.m_ViewPortWidth, rts.m_ViewPortHeight);
    }
    
    void GLContext::RenderTargetEnd() {
        
        RenderTargetState& previous = RenderTargetState::renderTargetStack.top();
        previous.Destroy();
        
        RenderTargetState::renderTargetStack.pop();
        
        // if there are still render targets states
        if(RenderTargetState::renderTargetStack.size() > 0) {
            
            RenderTargetState& renderTargetState = RenderTargetState::renderTargetStack.top();
            
            // set render targets
            renderTargetState.SetAsCurrentRenderTarget();
            
            // set viewport
            glViewport(renderTargetState.m_ViewPortX, renderTargetState.m_ViewPortY,
                       renderTargetState.m_ViewPortWidth, renderTargetState.m_ViewPortHeight);
        
        } else {
            // there are no render targets left so bind to back buffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    
    }

}
