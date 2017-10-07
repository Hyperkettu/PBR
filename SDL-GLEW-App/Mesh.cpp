//
//  Mesh.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "glm/glm.hpp"

#include "Application.h"
#include "Mesh.h"

namespace Fox {

    template<>
    FMesh<Vertex>::FMesh(std::vector<Vertex>& vertices, GLenum usage) {
     
        // copy data to vector
        m_Vertices.resize(vertices.size());
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        
        // create vertex buffer object
        glGenBuffers(1, &m_Vbo);
        
        // create vertex array
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        // fill buffer with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), (const GLvoid*) vertices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex),(GLvoid*) 0);// (GLvoid*)&Vertex::m_Position);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (3 * sizeof(GLfloat)));
        // &Vertex::m_Normal;
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (6 * sizeof(GLfloat)));
        // &Vertex::m_TexCoords;
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
        
    }
    
    template<>
    FMesh<VertexP>::FMesh(std::vector<VertexP>& vertices, GLenum usage) {
        
        // copy data to vector
        m_Vertices.resize(vertices.size());
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        
        // create vertex buffer object
        glGenBuffers(1, &m_Vbo);
        
        // create vertex array
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        // fill buffer with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexP) * vertices.size(), (const GLvoid*) vertices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexP),(GLvoid*) 0);// (GLvoid*)&Vertex::m_Position);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
    }
    
    template<>
    Mesh<Vertex>::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLenum usage){
        
        m_Vertices.resize(vertices.size());
        m_Indices.resize(indices.size());
        
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        std::copy(indices.begin(), indices.end(), m_Indices.begin());
        
        // generate vertex and index buffer objects
        glGenBuffers(1, &m_Vbo);
        glGenBuffers(1, &m_Ibo);
        
        // create vertex array object
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), (const GLvoid*) m_Vertices.data(), usage);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), (const GLvoid*) m_Indices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (6 * sizeof(GLfloat)));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
    
    template<>
    Mesh<VertexPNTTB>::Mesh(std::vector<VertexPNTTB>& vertices, std::vector<GLuint>& indices, GLenum usage){
    
        m_Vertices.resize(vertices.size());
        m_Indices.resize(indices.size());
        
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        std::copy(indices.begin(), indices.end(), m_Indices.begin());
        
        // generate vertex and index buffer objects
        glGenBuffers(1, &m_Vbo);
        glGenBuffers(1, &m_Ibo);
        
        // create vertex array object
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPNTTB) * m_Vertices.size(), (const GLvoid*) m_Vertices.data(), usage);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), (const GLvoid*) m_Indices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexPNTTB), (GLvoid*) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexPNTTB), (GLvoid*) (3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(VertexPNTTB), (GLvoid*) (6 * sizeof(GLfloat)));
        glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(VertexPNTTB), (GLvoid*) (8 * sizeof(GLfloat)));
        glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(VertexPNTTB), (GLvoid*) (11 * sizeof(GLfloat)));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        
        glBindVertexArray(0);
    }
    
    template<class V>
    void FMesh<V>::draw(GLContext* gl){
        
        // bind all existing textures
        for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
            
            Texture* texture = m_Material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                
                gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
            
        }
        
        glActiveTexture(GL_TEXTURE0);
        
        //  if(gl->getCurrentShader()->hasUniform("material.shininess"))
        gl->getCurrentShader()->setFloat("material.shininess", m_Material.m_Shininess);
        
        // render this mesh
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        glBindVertexArray(0);
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
    }
    
    template <class V>
    void FMesh<V>::draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions){
        
        // bind all existing textures
        for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
            
            Texture* texture = m_Material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
            
        }
        
        glActiveTexture(GL_TEXTURE0);
        
        glBindVertexArray(m_Vao);
        
        gl->getCurrentShader()->setFloat("material.shininess", m_Material.m_Shininess);
        
        // render n times
        for(GLuint i = 0; i < n; i++)
        {
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, positions[i]);
            //  GLfloat angle = 20.0f * i;
            //  model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            gl->getCurrentShader()->setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        }
        
        glBindVertexArray(0);
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    template<class V>
    void Mesh<V>::draw(GLContext* gl){
        
       // gl->getCurrentShader()->setFloat("material.shininess", m_Material.m_Shininess);
        
        glBindVertexArray(m_Vao);
        
        if(m_Material.m_Textures[Texture::TextureType::Displacement] != nullptr){
            glDrawElements(GL_PATCHES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        } else
            glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0); //BUFFER_OFFSET(0));
      
        glBindVertexArray(0);
    }
    
    template<class V>
    void Mesh<V>::draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions){
        
        // bind all existing textures
        for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
            
            
            Texture* texture = m_Material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
            
        }
        
        glActiveTexture(GL_TEXTURE0);
        
        glBindVertexArray(m_Vao);
        
        gl->getCurrentShader()->setFloat("material.shininess", m_Material.m_Shininess);
        
        
        
        // render n times
        for(GLuint i = 0; i < n; i++)
        {
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, positions[i]);
            gl->getCurrentShader()->setMat4("model", model);
            
            gl->getCurrentRenderContext().updateFrustum(model);
            
            // use frustum culling and bounding volume to determine if sphere is inside
            if(gl->getCurrentRenderContext().m_Frustum.sphereIsInsideFrustum(m_BoundingSphere.m_Center, m_BoundingSphere.m_Radius)){
                glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0);
                
            }
        }
        
        glBindVertexArray(0);
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    template<class V>
     void Mesh<V>::calculateTangents(std::vector<VertexPNTTB>& vertices, const std::vector<GLuint>& indices){
        
        for(int i = 0; i < indices.size(); i+=3){
            
            VertexPNTTB& v = vertices[indices[i]];
            VertexPNTTB& v2 = vertices[indices[i + 1]];
            VertexPNTTB& v3 = vertices[indices[i + 2]];
            
            glm::vec3 edge1 = v2.m_Position - v.m_Position;
            glm::vec3 edge2 = v3.m_Position - v.m_Position;
            glm::vec2 deltaUV1 = v2.m_TexCoords - v.m_TexCoords;
            glm::vec2 deltaUV2 = v3.m_TexCoords - v.m_TexCoords;
            
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            
            glm::vec3 tangent;
            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent = glm::normalize(tangent);
            
            v.m_Tangent = tangent;
            v2.m_Tangent = tangent;
            v3.m_Tangent = tangent;
        }
        
    }
    
    template<class V>
    void Mesh<V>::calculateNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices){
        
        GLfloat* normal = new GLfloat[3 * vertices.size()];
        
        // initialize normal array with zeros
        for(GLuint i = 0; i < 3 * vertices.size(); i++){
            normal[i] = 0;
        }
        
        // calculate normals with cross product
        for (GLuint e = 0; e < indices.size(); e+=3) {
            
            Vertex& v = vertices[indices[e]];
            Vertex& v2 = vertices[indices[e + 1]];
            Vertex& v3 = vertices[indices[e + 2]];
            
            // triangle end points
            glm::vec3 a = v.m_Position;
            glm::vec3 b = v2.m_Position;
            glm::vec3 c = v3.m_Position;
            // cross product
            glm::vec3 n = glm::cross(c - b, a - b);
            
            // add normals to every participating vertex
            normal[3*indices[e]] += n.x;
            normal[3*indices[e] + 1] += n.y;
            normal[3*indices[e] + 2] += n.z;
            
            normal[3*indices[e+1]] += n.x;
            normal[3*indices[e+1] + 1] += n.y;
            normal[3*indices[e+1] + 2] += n.z;
            
            normal[3*indices[e+2]] += n.x;
            normal[3*indices[e+2] + 1] += n.y;
            normal[3*indices[e+2] + 2] += n.z;
            
        }
        
        // normalize normals
        for(GLuint i = 0; i < 3 * vertices.size(); i+=3){
            
            glm::vec3 n = glm::vec3(normal[i], normal[i+1], normal[i+2]);
            n = glm::normalize(n);
            
            normal[i] = n.x;
            normal[i+1] = n.y;
            normal[i+2] = n.z;
            
        }
        // set normals to vertices
        for(GLuint i = 0; i < vertices.size(); i++){
            
            Vertex& vertex = vertices[i];
            vertex.m_Normal = glm::vec3(normal[3*i], normal[3*i+1], normal[3*i+2]);
        }
        
        // release array memory
        delete[] normal;
        
    }
    
    
    
}
