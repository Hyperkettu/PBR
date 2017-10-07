//
//  RenderContext.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/31/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "RenderContext.h"

namespace Fox {
    

    RenderContext::RenderContext(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat fov, GLfloat near, GLfloat far) : m_ViewPortX(x), m_ViewPortY(y), m_ViewPortWidth(width), m_ViewPortHeight(height), m_FiewOfView(fov), m_Near(near), m_Far(far) {
    
        m_ViewPort = Fox::viewport(m_ViewPortX, m_ViewPortY, m_ViewPortWidth, m_ViewPortHeight);
        m_Projection = glm::perspective(m_FiewOfView, (GLfloat) m_ViewPortWidth/ (GLfloat) m_ViewPortHeight, m_Near, m_Far);
        
    }

}
