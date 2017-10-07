//
//  Frustum.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/11/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Frustum.h"
#include "Matrix.hpp"

namespace Fox {
    
    void Frustum::updateFrustum(glm::mat4& projection, glm::mat4 view, glm::mat4 model) {
        
        glm::mat4 clip = projection * view * model;
        
        // right plane
     /*   m_Frustum[RIGHT][A] = clip[3][0] - clip[0][0];
        m_Frustum[RIGHT][B] = clip[3][1] - clip[0][1];
        m_Frustum[RIGHT][C] = clip[3][2] - clip[0][2];
        m_Frustum[RIGHT][D] = clip[3][3] - clip[0][3];*/
        
        m_Frustum[RIGHT][A] = clip[0][3] - clip[0][0];
        m_Frustum[RIGHT][B] = clip[1][3] - clip[1][0];
        m_Frustum[RIGHT][C] = clip[2][3] - clip[2][0];
        m_Frustum[RIGHT][D] = clip[3][3] - clip[3][0];
        
        normalizePlane(m_Frustum, RIGHT);
        
        // left plane
        /*m_Frustum[LEFT][A] = clip[3][0] + clip[0][0];
        m_Frustum[LEFT][B] = clip[3][1] + clip[0][1];
        m_Frustum[LEFT][C] = clip[3][2] + clip[0][2];
        m_Frustum[LEFT][D] = clip[3][3] + clip[0][3];*/
        
        m_Frustum[LEFT][A] = clip[0][3] + clip[0][0];
        m_Frustum[LEFT][B] = clip[1][3] + clip[1][0];
        m_Frustum[LEFT][C] = clip[2][3] + clip[2][0];
        m_Frustum[LEFT][D] = clip[3][3] + clip[3][0];
        
        normalizePlane(m_Frustum, LEFT);
        
        // bottom plane
        /*m_Frustum[BOTTOM][A] = clip[3][0] + clip[1][0];
        m_Frustum[BOTTOM][B] = clip[3][1] + clip[1][1];
        m_Frustum[BOTTOM][C] = clip[3][2] + clip[1][2];
        m_Frustum[BOTTOM][D] = clip[3][3] + clip[1][3];*/
        
        m_Frustum[BOTTOM][A] = clip[0][3] + clip[0][1];
        m_Frustum[BOTTOM][B] = clip[1][3] + clip[1][1];
        m_Frustum[BOTTOM][C] = clip[2][3] + clip[2][1];
        m_Frustum[BOTTOM][D] = clip[3][3] + clip[3][1];
        
        normalizePlane(m_Frustum, BOTTOM);
        
        // top plane
        /*m_Frustum[TOP][A] = clip[3][0] - clip[1][0];
        m_Frustum[TOP][B] = clip[3][1] - clip[1][1];
        m_Frustum[TOP][C] = clip[3][2] - clip[1][2];
        m_Frustum[TOP][D] = clip[3][3] - clip[1][3];*/
        
        m_Frustum[TOP][A] = clip[0][3] - clip[0][1];
        m_Frustum[TOP][B] = clip[1][3] - clip[1][1];
        m_Frustum[TOP][C] = clip[2][3] - clip[2][1];
        m_Frustum[TOP][D] = clip[3][3] - clip[3][1];
        
        normalizePlane(m_Frustum, TOP);
        
        // far plane
        /*m_Frustum[BACK][A] = clip[3][0] - clip[2][0];
        m_Frustum[BACK][B] = clip[3][1] - clip[2][1];
        m_Frustum[BACK][C] = clip[3][2] - clip[2][2];
        m_Frustum[BACK][D] = clip[3][3] - clip[2][3];*/
        
        m_Frustum[BACK][A] = clip[0][3] - clip[0][2];
        m_Frustum[BACK][B] = clip[1][3] - clip[1][2];
        m_Frustum[BACK][C] = clip[2][3] - clip[2][2];
        m_Frustum[BACK][D] = clip[3][3] - clip[3][2];
        
        normalizePlane(m_Frustum, BACK);
        
        // near plane
     /*   m_Frustum[FRONT][A] = clip[3][0] + clip[2][0];
        m_Frustum[FRONT][B] = clip[3][1] + clip[2][1];
        m_Frustum[FRONT][C] = clip[3][2] + clip[2][2];
        m_Frustum[FRONT][D] = clip[3][3] + clip[2][3];*/
        
        m_Frustum[FRONT][A] = clip[0][3] + clip[0][2];
        m_Frustum[FRONT][B] = clip[1][3] + clip[1][2];
        m_Frustum[FRONT][C] = clip[2][3] + clip[2][2];
        m_Frustum[FRONT][D] = clip[3][3] + clip[3][2];
        
        normalizePlane(m_Frustum, FRONT);
        
    }
    
    bool Frustum::pointIsInsideFrustum(const glm::vec3& point) {
        
        // check all planes
        for(GLuint i = 0; i < 6; i++ )
        {
            if(m_Frustum[i][A] * point.x + m_Frustum[i][B] * point.y + m_Frustum[i][C] * point.z + m_Frustum[i][D] <= 0)
            {
                return false;
            }
        }
        // point is inside frustum
        return true;
    }
    

    bool Frustum::sphereIsInsideFrustum(const glm::vec3& center, GLfloat radius) {
    
        // check all planes
        for(GLuint i = 0; i < 6; i++ )
        {
            if( m_Frustum[i][A] * center.x + m_Frustum[i][B] * center.y + m_Frustum[i][C] * center.z + m_Frustum[i][D] <= -radius ) {
                return false;
            }
        }
        // sphere is inside frustum
        return true;
    }
    

    bool Frustum::cubeIsInsideFrustum(const glm::vec3& center, float size) {
        
        // check all planes
        for(GLuint i = 0; i < 6; i++ )
        {
            if(m_Frustum[i][A] * (center.x - size) + m_Frustum[i][B] * (center.y - size) + m_Frustum[i][C] * (center.z - size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + size) + m_Frustum[i][B] * (center.y - size) + m_Frustum[i][C] * (center.z - size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - size) + m_Frustum[i][B] * (center.y + size) + m_Frustum[i][C] * (center.z - size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + size) + m_Frustum[i][B] * (center.y + size) + m_Frustum[i][C] * (center.z - size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - size) + m_Frustum[i][B] * (center.y - size) + m_Frustum[i][C] * (center.z + size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + size) + m_Frustum[i][B] * (center.y - size) + m_Frustum[i][C] * (center.z + size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - size) + m_Frustum[i][B] * (center.y + size) + m_Frustum[i][C] * (center.z + size) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + size) + m_Frustum[i][B] * (center.y + size) + m_Frustum[i][C] * (center.z + size) + m_Frustum[i][D] > 0)
                continue;
            
            return false;
        }
        
        return true;
    }
    
    
    bool Frustum::boxIsInsideFrustum(const glm::vec3& center, const glm::vec3 size) {
        
        GLfloat xSize = size.x;
        GLfloat ySize = size.y;
        GLfloat zSize = size.z;
        
        // check all planes
        for(int i = 0; i < 6; i++ )
        {
            if(m_Frustum[i][A] * (center.x - xSize) + m_Frustum[i][B] * (center.y - ySize) + m_Frustum[i][C] * (center.z - zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + xSize) + m_Frustum[i][B] * (center.y - ySize) + m_Frustum[i][C] * (center.z - zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - xSize) + m_Frustum[i][B] * (center.y + ySize) + m_Frustum[i][C] * (center.z - zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + xSize) + m_Frustum[i][B] * (center.y + ySize) + m_Frustum[i][C] * (center.z - zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - xSize) + m_Frustum[i][B] * (center.y - ySize) + m_Frustum[i][C] * (center.z + zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + xSize) + m_Frustum[i][B] * (center.y - ySize) + m_Frustum[i][C] * (center.z + zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x - xSize) + m_Frustum[i][B] * (center.y + ySize) + m_Frustum[i][C] * (center.z + zSize) + m_Frustum[i][D] > 0)
                continue;
            if(m_Frustum[i][A] * (center.x + xSize) + m_Frustum[i][B] * (center.y + ySize) + m_Frustum[i][C] * (center.z + zSize) + m_Frustum[i][D] > 0)
                continue;
            
            return false;
        }
        // box is inside frustum
        return true;
    }
    
}
