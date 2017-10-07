//
//  Vector.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Vector_h
#define Vector_h

#include <iostream>

#include "glm/glm.hpp"

namespace Fox {
    
    /**
     * Calculates cartesian vector from given vector, thus divides with w
     *
     * @param vector operated
     * @return cartesian result vector
     */
    static glm::vec3 cartesian(const glm::vec4& vector){
        return glm::vec3(vector/vector.w);
    
    }
    
    static void printVec(const glm::vec3& vec){
        std::cout << "vec3: " << vec.x << " " << vec.y << " " << vec.z << std::endl;
    }
    
    static void printVec(const glm::vec2& vec){
        std::cout << "vec2: " << vec.x << " " << vec.y << " " << std::endl;
    }
  
}


#endif /* Vector_h */
