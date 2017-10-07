//
//  RenderContext.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/31/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef RenderContext_h
#define RenderContext_h

#include "Camera.h"
#include "Matrix.hpp"
#include "Vector.h"
#include "Ray.hpp"
#include "Frustum.h"

namespace Fox {
    
    /**
     * Render context contains information about how a scene is to be rendered
     */
    class RenderContext {
    
    public:
        
        RenderContext(){}
        
        /**
         * Creates a render context with given parameters
         *
         * @param x Viewport x
         * @param y Viewport y
         * @param width Viewport width
         * @param height Viewport height
         * @param fov Field of view
         * @param near Near clip plane
         * @param far Far clip plane
         */
        RenderContext(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat fov, GLfloat near, GLfloat far);
        
        /**
         * Casts a ray through pixel coordinates, coordinates are from bottom left corner
         * @param x coordinate
         * @param y coordinate
         * @return intersection
         */
        glm::vec3 castRay(GLfloat x, GLfloat y){
            
            glm::vec4 throughWhichPixel = glm::vec4(x, y, 1.0f, 1.0f);
            
            // calculate unProject matrix
            glm::mat4 P = m_ViewPort * m_Projection * m_Camera.view();
            glm::mat4 iP = glm::inverse(P); // camera->base() * projection^-1 * viewPort^-1 * throughWhichPixel
            
            // calculate ray direction
            glm::vec3 endPoint = cartesian(iP*throughWhichPixel);
            glm::vec3 d = endPoint - m_Camera.m_Position;
            
            // calculate intersection
            GLfloat t = -m_Camera.m_Position.y/d.y;
            
            // form ray
            Ray ray = Ray(m_Camera.m_Position, d);
            
            return ray(t);
            
            
        }
        
        /**
         * Updates view frustum including model matrix
         */
        void updateFrustum(glm::mat4 model){
            m_Frustum.updateFrustum(m_Projection, m_Camera.view(), model);
        }
        
        Camera m_Camera; ///< camera of this render context
        Frustum m_Frustum; ///< frustum culling
        
        GLint m_ViewPortX, m_ViewPortY, m_ViewPortWidth, m_ViewPortHeight; ///< viewport variables
        glm::mat4 m_ViewPort; /// < viewport matrix
        
        GLfloat m_FiewOfView; ///< field of view of the camera
        GLfloat m_Near; ///< near clip plane
        GLfloat m_Far; ///< far clip plane
        glm::mat4 m_Projection; ///< projection matrix
        
        // render target
    };
}


#endif /* RenderContext_h */
