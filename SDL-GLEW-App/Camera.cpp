//
//  Camera.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/28/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Camera.h"

namespace Fox {

void Camera::setPosition(const glm::vec3& newPosition){
    m_Position = newPosition;
    glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
    
}
