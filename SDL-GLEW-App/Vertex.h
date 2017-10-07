//
//  Vertex.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include "glm/glm.hpp"

namespace Fox {
    
    class VertexP {
    public:
        
        VertexP() {}
        
        VertexP(const glm::vec3& position) : m_Position(position) {}
        
        glm::vec3 m_Position;
    };
    
    class VertexPC {
    public:
        
        VertexPC() {}
        
        VertexPC(const glm::vec3& position, const glm::vec3& color) : m_Position(position), m_Color(color) {}
        
        glm::vec3 m_Position;
        glm::vec3 m_Color;
    };
    
    class VertexPN {
    public:
        
        VertexPN() {}
        
        VertexPN(const glm::vec3& position, const glm::vec3& normal) : m_Position(position), m_Normal(normal) {}
        
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
    };
    
    class VertexPNC {
    public:
        
        VertexPNC() {}
        
        VertexPNC(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color) : m_Position(position), m_Normal(normal), m_Color(color) {}
        
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec3 m_Color;
    };
    
    class VertexPNT {
    public:
        
        VertexPNT() {}
        
        VertexPNT(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& tex) : m_Position(position), m_Normal(normal), m_TexCoords(tex) {}

        
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec2 m_TexCoords;
    };
    
    class VertexPNTC {
    public:
        
        VertexPNTC(){}
        
        VertexPNTC(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& tex, const glm::vec3& color) : m_Position(position), m_Normal(normal), m_TexCoords(tex), m_Color(color) {}
        
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec2 m_TexCoords;
        glm::vec3 m_Color;
    };
    
    class VertexPNTTB {
    
    public:
        VertexPNTTB(){}
        
        VertexPNTTB(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& tex, const glm::vec3& tangent, const glm::vec3 bitangent) : m_Position(position), m_Normal(normal), m_TexCoords(tex), m_Tangent(tangent), m_Bitangent(bitangent) {}
        
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec2 m_TexCoords;
        glm::vec3 m_Tangent;
        glm::vec3 m_Bitangent;
    
    };
    
    typedef VertexPNT Vertex;
}

#endif /* Vertex_h */
