//
//  Light.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/19/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Light_h
#define Light_h

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fox {
    
    class Light {
    
    public:
        
        Light() {}
    
    };
    
    class DirectionalLight : public Light {
    
    public:
        
        DirectionalLight(){}
        
        glm::vec3 m_Ambient;
        glm::vec3 m_Diffuse;
        glm::vec3 m_Specular;
        
        glm::vec3 m_Direction;
        
        glm::vec3 m_Color;
    
    };
}


#endif /* Light_h */
