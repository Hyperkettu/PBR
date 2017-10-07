//
//  Frustum.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/11/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Frustum_h
#define Frustum_h

#include <GL/glew.h>

#include <iostream>

#include "glm/glm.hpp"

namespace Fox {

/**
 * Frustum used for frustum culling
 */
class Frustum {
    
public:
    
    enum FrustumSide
    {
        RIGHT	= 0,		// The right side of the frustum
        LEFT	= 1,		// The left side of the frustum
        BOTTOM	= 2,		// The bottom side of the frustum
        TOP		= 3,		// The top side of the frustum
        BACK	= 4,		// The back side of the frustum
        FRONT	= 5			// The front side of the frustum
    };
    
    enum Plane
    {
        A = 0,				// X value of the plane's normal
        B = 1,				// Y value of the plane's normal
        C = 2,				// Z value of the plane's normal
        D = 3				// The distance that the plane is from the origin
    };
    
    /**
     * Updates the frustum. Should be called every time camera moves
     */
    void updateFrustum(glm::mat4& projection, glm::mat4 view, glm::mat4 model);
   
    /**
     * Checks if a 3d point is inside frustum
     *
     * @param point Point to be checked
     * @return if point is inside frustum
     */
    bool pointIsInsideFrustum(const glm::vec3& point);
    
    /**
     * Checks if a sphere is inside the frustum
     *
     * @param center Center of the sphere
     * @param radius Radius of the sphere
     * @return if sphere is inside frustum
     */
    bool sphereIsInsideFrustum(const glm::vec3& center, GLfloat radius);
    
    /**
     * Checks if cube is inside frustum
     *
     * @param center Center of the cube
     * @param size Half the length of the cube
     */
    bool cubeIsInsideFrustum(const glm::vec3& center, float size);
    
    /**
     * Checks if box is inside frustum
     *
     * @param center Center of the box
     * @param size Extents of the box
     */
    bool boxIsInsideFrustum(const glm::vec3& center, const glm::vec3 size);
    
    
    /**
     * Normalizes the frustum
     *
     * @param frustum Frustum to be normalized 
     * @param side Side of the frustum to be normalized
     */
    static void normalizePlane(GLfloat frustum[6][4], GLint side) {
        
        GLfloat magnitude = (GLfloat) sqrt( frustum[side][A] * frustum[side][A] +
                                      frustum[side][B] * frustum[side][B] +
                                      frustum[side][C] * frustum[side][C] );
        
        frustum[side][A] /= magnitude;
        frustum[side][B] /= magnitude;
        frustum[side][C] /= magnitude;
        frustum[side][D] /= magnitude;
    }
    
    
private:
    
    GLfloat m_Frustum[6][4]; ///< A, B, C, D values of a plane for each side of the frustum
};
    
}


#endif /* Frustum_h */
