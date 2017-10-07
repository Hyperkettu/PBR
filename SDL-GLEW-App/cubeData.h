//
//  cubeData.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef cubeData_h
#define cubeData_h

#include <vector>

#include "glm/glm.hpp"

#include "Vertex.h"

namespace Fox {
    
    static GLfloat cubeData[] = {
        
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    
    
    
    
    template<class V> static std::vector<V> convertCubeData(){
        return std::vector<V>();
    }

    template<> static
    std::vector<Vertex> convertCubeData() {
        
        std::vector<Vertex> data;
        
        for(int i = 0; i < 6*8*6; i+= 8) {
            
            glm::vec3 position(cubeData[i], cubeData[i+1], cubeData[i+2]);
            glm::vec3 normal(cubeData[i+3], cubeData[i+4], cubeData[i+5]);
            glm::vec2 uvs(cubeData[i+6], cubeData[i+7]);
            
            glm::vec3 tangent;
            
            
            Vertex vertex(position, normal, uvs);
            data.push_back(vertex);
        }
        return data;
    }
    
    template<> static 
    std::vector<VertexP> convertCubeData() {
        
        std::vector<VertexP> data;
        
        for(int i = 0; i < 6*8*6; i+= 8) {
            glm::vec3 position(cubeData[i], cubeData[i+1], cubeData[i+2]);
            VertexP vertex(position);
            data.push_back(vertex);
        }
        return data;
    }
    
    template<> static
    std::vector<VertexPNTTB> convertCubeData() {
        
        std::vector<VertexPNTTB> vertices;
        
     //   for(int i = 0; i < 6*8*6; i+= 8) {
        
        VertexPNTTB vertex;
    
        //front
        vertex.m_Position = glm::vec3(-0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f,  0.0f, 1.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f,  0.0f, 1.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f, 0.0f, 1.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        //right
        vertex.m_Position = glm::vec3(0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(1.0f, 0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(1.0f,  0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(1.0f,  0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(1.0f, 0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        // back
        vertex.m_Position = glm::vec3(-0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f, 0.0f, -1.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f,  0.0f, -1.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f,  0.0f, -1.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f, 0.0f, -1.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        // left
        vertex.m_Position = glm::vec3(-0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(-1.0f,  0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(-0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(-1.0f,  0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        // up
        vertex.m_Position = glm::vec3(-0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, 0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f,  1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f,  1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, 0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        // bottom
        vertex.m_Position = glm::vec3(-0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f, -1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(0.5, -0.5, -0.5);
        vertex.m_Normal = glm::vec3(0.0f,  -1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 0.0f);
        vertices.push_back(vertex);
        
        
        vertex.m_Position = glm::vec3(0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f,  -1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(1.0f, 1.0f);
        vertices.push_back(vertex);
        
        vertex.m_Position = glm::vec3(-0.5, -0.5, 0.5);
        vertex.m_Normal = glm::vec3(0.0f, -1.0f, 0.0f);
        vertex.m_TexCoords = glm::vec2(0.0f, 1.0f);
        vertices.push_back(vertex);
        
        
       // glm::vec3 position(cubeData[i], cubeData[i+1], cubeData[i+2]);
       // glm::vec3 normal(cubeData[i+3], cubeData[i+4], cubeData[i+5]);
       // glm::vec2 uvs(cubeData[i+6], cubeData[i+7]);
       // VertexPNTTB vertex(position, normal, uvs, normal, normal);
       // data.push_back(vertex);
      //  }
        
        
        
        return vertices;
    }
}


#endif /* cubeData_h */
