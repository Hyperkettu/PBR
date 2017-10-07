//
//  Ray.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include <GL/glew.h>

#include "glm/glm.hpp"

namespace Fox {
   
    /**
     * Class ray represents a ray in 3D space
     */
    class Ray {
        
    public:
        /**
         * Class Constructor
         */
        Ray(){}
        
        /**
         * Class contsructor
         * @param startingPoint
         * @param direction
         */
        Ray(const glm::vec3& startingPoint, const glm::vec3& direction);
        
        /**
         * Calculates a point on the ray
         * @param t variable
         * @return calculated point
         */
        inline glm::vec3 operator() (GLfloat t) {
            return p + t*d;
        }
        
    private:
        glm::vec3 p, d; ///< starting point and direction of the ray
    };
}

#endif /* Ray_hpp */
