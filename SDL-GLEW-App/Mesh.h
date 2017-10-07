//
//  Mesh.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

#include <GL/glew.h>

#include <vector>

#include "Vertex.h"
#include "cubeData.h"
#include "GLContext.h"
#include "Texture.h"
#include "TextureManager.h"
#include "BoundingVolume.h"

namespace Fox {
    
  //  static std::vector<const GLchar*> textureToUniformName = {"material.diffuse", "material.specular", nullptr, "material.normalMap"};
    
    static std::vector<const GLchar*> textureToUniformName = {"material.albedo", "material.normalMap", "material.roughness", "material.metallic", "material.ao", "displacementMap"};
    
    class MeshBase {
    public:
        
        MeshBase(){
            
            m_Material.m_Shininess = 2.0f;
            m_Material.m_Textures = std::vector<Texture*>(Texture::TextureType_Max);
            //m_Material.m_Shader = Shader::PBR;
          //  m_Material.m_Shader = Shader::BLINN_PHONG;
            m_Material.m_Shader = Shader::PBR_DISPLACEMENT;
        }
        
        
        
        virtual void draw(GLContext* gl){}
        
        virtual void drawToDepthBuffer(GLContext* gl){}
        
        virtual void drawWireframe(GLContext* gl){}
        
        
        /**
         * Adds a texture of certain type to this mesh's material
         *
         * @param texture Texture to be added
         */
        void addTexture(Texture* texture){
            GLuint index = (GLuint) texture->m_Type;
            m_Material.m_Textures[index] = texture;
            
        }
        
        /**
         * Computes the bounding sphere of this mesh
         *
         * @param vertices Vertices of the mesh
         */
        void computeBoundingSphere(std::vector<VertexPNTTB>& vertices){
            m_BoundingSphere = BoundingSphere(vertices);
        }
    
        // material class
        class Material {
        public:
        
            Material(){}
        
            GLfloat m_Shininess;
            std::vector<Texture*> m_Textures; /// all texture types
            Shader m_Shader;
        };
    
        Material m_Material; ///< material of the mesh
        BoundingSphere m_BoundingSphere; ///< bounding sphere
    };
        
template <class V = Vertex> class FMesh : public MeshBase {
    
    public:
    
    FMesh(){}
    
    ~FMesh(){
        glDeleteBuffers(1, &m_Vbo);
        glDeleteVertexArrays(1, &m_Vao);
    }
    
    FMesh(std::vector<V>& vertices, GLenum usage);
    
    void draw(GLContext* gl);
    
    void drawToDepthBuffer(GLContext* gl){
        // render this mesh
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        glBindVertexArray(0);
    
    }
    
    void draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions);
    
    void drawToDepthBuffer(GLContext* gl, std::vector<glm::vec3>& positions){
        
        glBindVertexArray(m_Vao);
        
        // render n times
        for(GLuint i = 0; i < positions.size(); i++)
        {
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, positions[i]);
            gl->getCurrentShader()->setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        }
        
        glBindVertexArray(0);
        
    }
    
    void drawWireframe(GLContext* gl){
        // enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        glBindVertexArray(0);
        
        // disable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    }
    
    std::vector<V> m_Vertices; ///< vertices
    std::vector<GLuint> m_Indices; ///< index data
    
private:
    
    GLuint m_Vao; ///< vertex array id
    GLuint m_Vbo; ///< vertex buffer object
    
    };
    
    template <class V = Vertex> class Mesh : public MeshBase {
    
    public:
        
        Mesh(){}
        
        Mesh(std::vector<V>& vertices, std::vector<GLuint>& indices, GLenum usage);
        
        ~Mesh(){
            
            glDeleteBuffers(1, &m_Vbo);
            glDeleteBuffers(1, &m_Ibo);
            glDeleteVertexArrays(1, &m_Vao);
            
        
        }
        
        void draw(GLContext* gl);
        
        void drawToDepthBuffer(GLContext* gl){
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0); //BUFFER_OFFSET(0));
            glBindVertexArray(0);
        }
        
        void drawWireframe(GLContext* gl){
            // enable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            
            // disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
        }
        
        void draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions);
        
        void drawToDepthBuffer(GLContext* gl, std::vector<glm::vec3>& positions) {
            
            glBindVertexArray(m_Vao);
            
            // render n times
            for(GLuint i = 0; i < positions.size(); i++)
            {
                glm::mat4 model = glm::mat4();
                model = glm::translate(model, positions[i]);
                gl->getCurrentShader()->setMat4("model", model);
                glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0);
            }
            
            glBindVertexArray(0);
        }
        
        
        
        /*
         * Updates the vertices
         *
         * @param vertices Vertices to add
         */
        void updateVertices(const std::vector<Vertex>& vertices) {
            
            glBindVertexArray(m_Vao);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertices.size(), (const GLvoid*)vertices.data());
            
            glBindVertexArray(0);
        }
        
        /**
         * Calculates tangents to the mesh based on vertex and index data
         *
         * @param vertices
         * @param indices
         */
        static void calculateTangents(std::vector<VertexPNTTB>& vertices, const std::vector<GLuint>& indices);
        
        
        /**
         * Subdivides the mesh
         */
        Mesh<VertexPNTTB>* subdivide(){
        
            
            std::vector<VertexPNTTB> vertices;
            std::vector<GLuint> indices;
            
            for(int i = 0; i < m_Indices.size(); i+=3) {
            
                VertexPNTTB v = (VertexPNTTB) m_Vertices[m_Indices[i]];
                VertexPNTTB v2 = (VertexPNTTB) m_Vertices[m_Indices[i + 1]];
                VertexPNTTB v3 = (VertexPNTTB) m_Vertices[m_Indices[i + 2]];
                
                VertexPNTTB nv1;
                nv1.m_Position = (v.m_Position + v2.m_Position)/2.0f;
                nv1.m_TexCoords = (v.m_TexCoords + v2.m_TexCoords)/2.0f;
                nv1.m_Normal = v.m_Normal;
                nv1.m_Tangent = v.m_Tangent;
                
                VertexPNTTB nv2;
                nv2.m_Position = (v.m_Position + v3.m_Position)/2.0f;
                nv2.m_TexCoords = (v.m_TexCoords + v3.m_TexCoords)/2.0f;
                nv2.m_Normal = v.m_Normal;
                nv2.m_Tangent = v.m_Tangent;
                
                VertexPNTTB nv3;
                nv3.m_Position = (v2.m_Position + v3.m_Position)/2.0f;
                nv3.m_TexCoords = (v2.m_TexCoords + v3.m_TexCoords)/2.0f;
                nv3.m_Normal = v.m_Normal;
                nv3.m_Tangent = v.m_Tangent;
                
                vertices.push_back(v); // 0
                vertices.push_back(v2); // 1
                vertices.push_back(v3); // 2
                vertices.push_back(nv1); // 3
                vertices.push_back(nv2); // 4
                vertices.push_back(nv3); // 5
                
                indices.push_back(2*i + 0);
                indices.push_back(2*i + 3);
                indices.push_back(2*i + 4);
                
                indices.push_back(2*i + 3);
                indices.push_back(2*i + 5);
                indices.push_back(2*i + 4);
                
                indices.push_back(2*i + 1);
                indices.push_back(2*i + 5);
                indices.push_back(2*i + 3);
                
                indices.push_back(2*i + 2);
                indices.push_back(2*i + 4);
                indices.push_back(2*i + 5);
                
            }
        
            return new Mesh<VertexPNTTB>(vertices, indices, GL_STATIC_DRAW);
        }
        
        /**
         * Calculates normals for vertices based on indices
         *
         * @param vertices Vertex array
         * @param indices Index array
         */
        static void calculateNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
        
        std::vector<V> m_Vertices; ///< vertices
        std::vector<GLuint> m_Indices; ///< index data
    
    private:
        GLuint m_Vao; ///< vertex array id
        GLuint m_Vbo; ///< vertex buffer object
        GLuint m_Ibo; ///< index buffer object
        
    };
    
    static MeshBase* import(const GLchar* fileName){
        return new MeshBase;
    }
    
}

#endif /* Mesh_h */
