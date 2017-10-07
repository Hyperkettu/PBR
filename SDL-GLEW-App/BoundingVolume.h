//
//  BoundingVolume.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/12/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef BoundingVolume_h
#define BoundingVolume_h

#include <GL/glew.h>

#include "glm/glm.hpp"

namespace Fox {

class BoundingSphere {

public:
    
    BoundingSphere() : m_Center(glm::vec3(0.0f)), m_Radius(100.0f){
    }
    
    /**
     * Constructs the bounding sphere based on vertex data
     */
    BoundingSphere(const std::vector<VertexPNTTB>& vertices){
        
        computeBoundingSphere(vertices);
    
    }
    
    /**
     * Computes the bounding sphere of the mesh
     * 
     * @param vertices Vertices of the mesh
     **/
    void computeBoundingSphere(const std::vector<VertexPNTTB>& vertices){
    
        glm::vec3 sum = glm::vec3(0,0,0);
        
        m_Radius = 100.0f;
        m_Center= glm::vec3(0.0f, 0.0f, 0.0f);
        
        // compute center of the sphere
        for(VertexPNTTB v : vertices) {
        
            sum += v.m_Position;
        }
        m_Center = sum / (GLfloat) (vertices.size());
        
        m_Radius = 0.0f;
        
        // compute radius
        for(VertexPNTTB v : vertices){
        
            GLfloat r = glm::length(v.m_Position - m_Center);
            // if we have a bigger radius, update radius
            if(r > m_Radius){
                m_Radius = r;
            }
        }
    }
    
    glm::vec3 m_Center; ///< center of the sphere
    GLfloat m_Radius; ///< radius of the sphere
    
    


};

}

#endif /* BoundingVolume_h */
