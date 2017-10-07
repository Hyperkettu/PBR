//
//  GeometryGenerator.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef GeometryGenerator_h
#define GeometryGenerator_h

#include "Mesh.h"
#include "Math.h"

namespace Fox {

    template<class V>
    static Mesh<V>* createIcosahedron(){
        
        const float X = 0.525731f;
        const float Z = 0.850651f;
        
        glm::vec3 pos[12] =
        {
            glm::vec3(-X, 0.0f, Z),  glm::vec3(X, 0.0f, Z),
            glm::vec3(-X, 0.0f, -Z), glm::vec3(X, 0.0f, -Z),
            glm::vec3(0.0f, Z, X),   glm::vec3(0.0f, Z, -X),
            glm::vec3(0.0f, -Z, X),  glm::vec3(0.0f, -Z, -X),
            glm::vec3(Z, X, 0.0f),   glm::vec3(-Z, X, 0.0f),
            glm::vec3(Z, -X, 0.0f),  glm::vec3(-Z, -X, 0.0f)
        };
        
        std::vector<GLuint> indices =
        {
            1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
            1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
            3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
            10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
        };
        
        
        std::vector<V> data;
        
        for(int i = 0; i < 12; i++){
            V v;
            v.m_Position = pos[i];
            data.push_back(v);
        }
        
        return new Mesh<V>(data, indices, GL_STATIC_DRAW);
    }


/**
 * Creates a geospehere
 *
 * @return Mesh
 */
template<class V>
static Mesh<V>* createGeosphere(){
    
    Mesh<V>* mesh = createIcosahedron<V>();
    
    for(int i = 0; i < 5; i++){
        mesh = mesh->subdivide();
    }
    
    for(int i = 0; i < mesh->m_Vertices.size(); i++){
        
        mesh->m_Vertices[i].m_Position = glm::normalize(mesh->m_Vertices[i].m_Position);
        mesh->m_Vertices[i].m_Normal = mesh->m_Vertices[i].m_Position;
        
        float theta = angleFromXY(mesh->m_Vertices[i].m_Position.x, mesh->m_Vertices[i].m_Position.z);
        float phi = acosf(mesh->m_Vertices[i].m_Position.y);
        
        float PI = 3.14159265358f;
        mesh->m_Vertices[i].m_TexCoords = glm::vec2(theta/(2*PI), phi/PI);
        
        float x = -sinf(theta)*sinf(phi);
        float y = 0.0f;
        float z = sinf(phi)*cosf(theta);
        mesh->m_Vertices[i].m_Tangent = glm::normalize(glm::vec3(x,y,z));
    }
    
    for(int i = 0; i < mesh->m_Indices.size(); i += 3) {
        V vertex = mesh->m_Vertices[mesh->m_Indices[i]];
        V vertex2 = mesh->m_Vertices[mesh->m_Indices[i+1]];
        V vertex3 = mesh->m_Vertices[mesh->m_Indices[i+2]];
        
        float diff = std::abs(vertex.m_TexCoords.x - vertex2.m_TexCoords.x);
        float diff2 = std::abs(vertex3.m_TexCoords.x - vertex2.m_TexCoords.x);
        float diff3 = std::abs(vertex.m_TexCoords.x - vertex3.m_TexCoords.x);
        
        float PI = 3.14159265358f;
        float phi = acosf(mesh->m_Vertices[mesh->m_Indices[i]].m_Position.y);
        
        if(phi > PI - 0.00001f) {
            
            float phi = acosf(vertex.m_Position.y);
            
            V vert;
            vert.m_Position = vertex.m_Position;
            vert.m_Normal = vertex.m_Normal;
            vert.m_Tangent = vertex.m_Tangent;
            
            phi -= 0.001f;
            float theta = angleFromXY(vert.m_Position.x, vert.m_Position.z);
            
            glm::vec3 centroid = vertex.m_Position + vertex3.m_Position + vertex3.m_Position;
            glm::vec3 tangent = glm::cross(vert.m_Normal, (centroid - vert.m_Position));
            vert.m_Tangent = glm::normalize(tangent);
            
            glm::vec3 pos = 0.995f *(vert.m_Position - centroid) + centroid;
            
            theta = angleFromXY(pos.x, pos.z);
            vert.m_TexCoords = glm::vec2(theta/(2*PI), phi/PI);
            
            mesh->m_Vertices.push_back(vert);
            mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
            
        }
        
        if(phi < 0.00001f) {
            
            float phi = acosf(vertex.m_Position.y);
            
            V vert;
            vert.m_Position = vertex.m_Position;
            vert.m_Normal = vertex.m_Normal;
            vert.m_Tangent = vertex.m_Tangent;
            
            phi -= 0.001f;
            float theta = angleFromXY(vert.m_Position.x, vert.m_Position.z);
            
            glm::vec3 centroid = vertex.m_Position + vertex3.m_Position + vertex3.m_Position;
            glm::vec3 tangent = glm::cross(vert.m_Normal, (centroid - vert.m_Position));
            vert.m_Tangent = glm::normalize(tangent);
            
            glm::vec3 pos = 0.995f *(vert.m_Position - centroid) + centroid;
            
            theta = angleFromXY(pos.x, pos.z);
            vert.m_TexCoords = glm::vec2(theta/(2*PI), phi/PI);
            
            mesh->m_Vertices.push_back(vert);
            mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
            
        }
        
        float threshold = 0.5f;
        
        if(diff > threshold || diff2 > threshold || diff3 > threshold){
            
            if(vertex.m_TexCoords.x == 0.0f && vertex2.m_TexCoords.x == 0.0f){
                
                V vert;
                vert.m_Position = vertex.m_Position;
                vert.m_TexCoords = glm::vec2(1.0f, vertex.m_TexCoords.y);
                vert.m_Normal = vertex.m_Normal;
                vert.m_Tangent = vertex.m_Tangent;
                
                mesh->m_Vertices.push_back(vert);
                mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                
                V vert2;
                vert2.m_Position = vertex2.m_Position;
                vert2.m_TexCoords = glm::vec2(1.0f, vertex2.m_TexCoords.y);
                vert2.m_Normal = vertex2.m_Normal;
                vert2.m_Tangent = vertex2.m_Tangent;
                mesh->m_Vertices.push_back(vert2);
                mesh->m_Indices[i+1] = mesh->m_Vertices.size() - 1;
                
            }
            
            if(vertex.m_TexCoords.x == 0.0f && vertex3.m_TexCoords.x == 0.0f){
                V vert;
                vert.m_Position = vertex.m_Position;
                vert.m_TexCoords = glm::vec2(1.0f, vertex.m_TexCoords.y);
                vert.m_Normal = vertex.m_Normal;
                vert.m_Tangent = vertex.m_Tangent;
                mesh->m_Vertices.push_back(vert);
                
                mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                
                V vert2;
                vert2.m_Position = vertex3.m_Position;
                vert2.m_TexCoords = glm::vec2(1.0f, vertex3.m_TexCoords.y);
                vert2.m_Normal = vertex3.m_Normal;
                vert2.m_Tangent = vertex3.m_Tangent;
                mesh->m_Vertices.push_back(vert2);
                mesh->m_Indices[i+2] = mesh->m_Vertices.size() - 1;
                
            }
            
            if(vertex.m_TexCoords.x == 0.0f && vertex2.m_TexCoords.x != 0.0f && vertex3.m_TexCoords.x != 0.0f){
                
                if(vertex2.m_TexCoords.x > 0.8f && vertex3.m_TexCoords.x > 0.8f){
                    
                    V vert;
                    vert.m_Position = vertex.m_Position;
                    vert.m_TexCoords = glm::vec2(1.0f, vertex.m_TexCoords.y);
                    vert.m_Normal = vertex.m_Normal;
                    vert.m_Tangent = vertex.m_Tangent;
                    mesh->m_Vertices.push_back(vert);
                    mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                }
                
                else {
                    
                    V vert;
                    vert.m_Position = vertex.m_Position;
                    vert.m_TexCoords = glm::vec2(1.0f, vertex.m_TexCoords.y);
                    vert.m_Normal = vertex.m_Normal;
                    vert.m_Tangent = vertex.m_Tangent;
                    
                    mesh->m_Vertices.push_back(vert);
                    mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                    
                    if(vertex2.m_TexCoords.x < 0.2f && vertex3.m_TexCoords.x > 0.8f){
                        
                        V vert;
                        vert.m_Position = vertex2.m_Position;
                        vert.m_TexCoords = glm::vec2(vertex2.m_TexCoords.x + 1.0f, vertex2.m_TexCoords.y);
                        vert.m_Normal = vertex2.m_Normal;
                        vert.m_Tangent = vertex2.m_Tangent;
                        
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i+1] = mesh->m_Vertices.size() - 1;
                        
                    } else if(vertex3.m_TexCoords.x < 0.2f && vertex2.m_TexCoords.x > 0.8f) {
                        
                        V vert;
                        vert.m_Position = vertex3.m_Position;
                        vert.m_TexCoords = glm::vec2(vertex3.m_TexCoords.x + 1.0f, vertex3.m_TexCoords.y);
                        vert.m_Normal = vertex3.m_Normal;
                        vert.m_Tangent = vertex3.m_Tangent;
                        
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i+2] = mesh->m_Vertices.size() - 1;
                        
                    }
                    
                }
                
            }
            
            if(vertex2.m_TexCoords.x == 0.0f && vertex.m_TexCoords.x != 0.0f && vertex3.m_TexCoords.x != 0.0f) {
                
                
                if(vertex.m_TexCoords.x > 0.8f && vertex3.m_TexCoords.x > 0.8f){
                    
                    V vert;
                    vert.m_Position = vertex2.m_Position;
                    vert.m_TexCoords = glm::vec2(1.0f, vertex2.m_TexCoords.y);
                    vert.m_Normal = vertex2.m_Normal;
                    vert.m_Tangent = vertex2.m_Tangent;
                    mesh->m_Vertices.push_back(vert);
                    mesh->m_Indices[i+1] = mesh->m_Vertices.size() - 1;
                    
                } else {
                    
                    if(vertex.m_TexCoords.x < 0.2f && vertex3.m_TexCoords.x > 0.8f){
                        
                        V vert;
                        vert.m_Position = vertex3.m_Position;
                        vert.m_TexCoords = glm::vec2(vertex3.m_TexCoords.x - 1.0f, vertex3.m_TexCoords.y);
                        vert.m_Normal = vertex3.m_Normal;
                        vert.m_Tangent = vertex3.m_Tangent;
                        
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i+2] = mesh->m_Vertices.size() - 1;
                        
                    } else if(vertex.m_TexCoords.x > 0.8f && vertex3.m_TexCoords.x < 0.2f){
                        
                        V vert;
                        vert.m_Position = vertex.m_Position;
                        vert.m_TexCoords = glm::vec2(vertex.m_TexCoords.x - 1.0f, vertex.m_TexCoords.y);
                        vert.m_Normal = vertex.m_Normal;
                        vert.m_Tangent = vertex.m_Tangent;
                        
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                        
                    }
                    
                }
                
                
            }
            
            if(vertex3.m_TexCoords.x == 0.0f && vertex.m_TexCoords.x != 0.0f && vertex2.m_TexCoords.x != 0.0f){
                
                if(vertex.m_TexCoords.x > 0.8f && vertex2.m_TexCoords.x > 0.8f){
                    V vert;
                    vert.m_Position = vertex3.m_Position;
                    vert.m_TexCoords = glm::vec2(1.0f, vertex3.m_TexCoords.y);
                    vert.m_Normal = vertex3.m_Normal;
                    vert.m_Tangent = vertex3.m_Tangent;
                    mesh->m_Vertices.push_back(vert);
                    mesh->m_Indices[i+2] = mesh->m_Vertices.size() - 1;
                } else {
                    
                    
                    if(vertex.m_TexCoords.x < 0.2f && vertex2.m_TexCoords.x > 0.8f){
                        
                        V src = vertex2;
                        
                        V vert;
                        vert.m_Position = src.m_Position;
                        vert.m_TexCoords = glm::vec2(src.m_TexCoords.x - 1.0f, src.m_TexCoords.y);
                        vert.m_Normal = src.m_Normal;
                        vert.m_Tangent = src.m_Tangent;
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i+1] = mesh->m_Vertices.size() - 1;
                        
                    } else if(vertex.m_TexCoords.x > 0.8f && vertex2.m_TexCoords.x < 0.2f){
                        
                        V src = vertex;
                        
                        V vert;
                        vert.m_Position = src.m_Position;
                        vert.m_TexCoords = glm::vec2(src.m_TexCoords.x - 1.0f, src.m_TexCoords.y);
                        vert.m_Normal = src.m_Normal;
                        vert.m_Tangent = src.m_Tangent;
                        mesh->m_Vertices.push_back(vert);
                        mesh->m_Indices[i] = mesh->m_Vertices.size() - 1;
                    }
                    
                }
            }
        }
        
    }
    
    return new Mesh<V>(mesh->m_Vertices, mesh->m_Indices, GL_STATIC_DRAW);
    
    }
    
    /**
     * Creates a cube
     *
     * @return cube mesh
     **/
    template <class V>
    static FMesh<V> createCube() {
        
        FMesh<V> mesh;
        
        std::vector<V> data = convertCubeData<V>();
        mesh = FMesh<V>(data, GL_STATIC_DRAW);
        
        return mesh;
    }
    
    
    template<class V> static
    Mesh<V>* createCubeIndexed(){
        
        std::vector<GLuint> indices =  {0,  1,  2,  0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9,  10, 8,  10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23};
        
        
        std::vector<V> data = convertCubeData<VertexPNTTB>();
        Mesh<V>::calculateTangents(data, indices);
        return new Mesh<V>(data, indices, GL_STATIC_DRAW);
        //Mesh<V> mesh = Mesh<V>(data, indices, GL_STATIC_DRAW);
        //return mesh;
        
    }
    
    /**
     * Generates plane vertex data
     *
     * @param vertex data
     */
    template<class V> static std::vector<V> getGroundData(const GLchar* filePath, GLint* width, GLint* height){
        return std::vector<V>();
    }
    
    /**
     * Generates plane index data
     *
     * @return index data
     */
    template<class V> static std::vector<V> getPlaneIndices(GLint w, GLint h){
        return std::vector<V>();
    }
    
    
    /**
     * Generates ground vertex data for type Vertex based on height map
     *
     * @return vertex data
     */
    template<> static
    std::vector<Vertex> getGroundData(const GLchar* filePath, GLint* width, GLint* height) {
        
        std::vector<Vertex> data;
        
        GLfloat dimension = 1.0f;
        
        SDL_Surface* image = Texture::readTexture(filePath);
        
        Uint8 r,g,b;
        
        Uint32* p = (Uint32*) image->pixels;
        
        GLint w = image->w;
        GLint h = image->h;
        *width = w;
        *height = h;
        
        for(int i = -w/2; i <= w/2; i++){
            
            for(int j = - h/2; j <= h/2; j++){
                
                GLint x = i+w/2;
                GLint y = j+h/2;
                
                if(x == w){
                    x--;
                }
                
                if(y == h){
                    y--;
                }
                
                // sample height map
                SDL_GetRGB(p[y*image->w + x], image->format, &r, &g, &b);
                
                GLint heightMap = (GLint)(0.21f * r + 0.72f * g + 0.07f * b);
                
                //GLint heightMap = (GLint) r;
                GLfloat h = ((GLfloat)(heightMap/255.0f) * 10.0f - 10.0f ) * 10;
                
                Vertex v;
                v.m_Position = glm::vec3(i*dimension, h, j*dimension);
                
                
                v.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v.m_TexCoords = glm::vec2((GLfloat) (i + 5), (GLfloat)(j + 5));
                data.push_back(v);
            }
        }
        
        SDL_FreeSurface(image);
        
        return data;
    }
    
    /**
     * Generates plane index data
     *
     * @return index data
     */
    template<> static
    std::vector<GLuint> getPlaneIndices(GLint w, GLint h){
        
        std::vector<GLuint> indices;
        
        for(int i = 0; i < w; i++){
            for(int j = 0; j < h; j++){
                // first triangle
                indices.push_back((h+1)*i + j);
                indices.push_back((h+1) * (i+1) + j + 1);   // swap
                indices.push_back((h+1) * (i+1) + j);      // swap
                // second triangle
                indices.push_back((h+1)*i + j);
                indices.push_back((h+1)*i + j + 1); // swap2
                indices.push_back((h+1) * (i+1) + j + 1); // swap2
                
            }
            
            /*   for(int i = 0; i < indices.size(); i++){
             std::cout << indices[i] << std::endl;
             }*/
        }
        return indices;
    }
    
    template<class V> static
    std::vector<V> getCylinderData(GLuint number, GLfloat height, GLfloat radius) {
        return std::vector<V>();
    }
    
    template<class V> static
    std::vector<V> getCylinderIndices(GLuint number) {
        return std::vector<V>();
    }
    
    template<> static
    std::vector<Vertex> getCylinderData(GLuint number, GLfloat height, GLfloat radius) {
        
        std::vector<Vertex> vertices;
        
        GLfloat deltaAngle =  2 * glm::pi<GLfloat>()/(GLfloat)(number);
        
        for(GLuint face = 0; face <= number; face++){
            
            Vertex vertex, vertex2;
            
            GLfloat x = radius * cos(face*deltaAngle);
            GLfloat z = radius * sin(face*deltaAngle);
            
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
            
            glm::vec3 pos = glm::vec3(x, -height/2.0f, z);
            glm::vec3 pos2 = glm::vec3(x, height/2.0f, z);
            
            if(x == 0 && z == 0){
                std::cout << face << std::endl;
            }
            
            vertex.m_Position = pos;
            vertex.m_Normal = normal;
            vertex.m_TexCoords = glm::vec2(1/(GLfloat)(number)*face, 0.0f);
            
            vertices.push_back(vertex);
            
            vertex2.m_Position = pos2;
            vertex2.m_Normal = normal;
            vertex2.m_TexCoords = glm::vec2(1/(GLfloat)(number)*face, 1.0f);
            vertices.push_back(vertex2);
        }
        
        
        return vertices;
    }
    
    template<> static
    std::vector<GLuint> getCylinderIndices(GLuint number) {
        
        std::vector<GLuint> indices;
        
        for(int i = 0; i <= number; i++){
            // first triangle
            indices.push_back(2 * i);
            indices.push_back(2 * (i + 1));
            indices.push_back(2 * (i + 1) + 1);
            // second triangle
            indices.push_back(2 * i);
            indices.push_back((2 * (i + 1)) + 1);
            indices.push_back(2 * i + 1);
        }
        
        return indices;
    }
    
    template<class V>
    static std::vector<V> sphere(int sides, GLfloat r){
        return std::vector<V>();
    }
    
    /**
     * Calculates vertices of a sphere with sides^2 faces and r radius
     */
    template<>
    static std::vector<Vertex> sphere(int sides, GLfloat r){
        
        std::vector<Vertex> vertices;
        
        GLfloat angle = 0;
        GLfloat PI = glm::pi<GLfloat>();
        GLfloat dAngle = 2*PI/static_cast<GLfloat>(sides);
        GLfloat dh = 2*r/static_cast<GLfloat>(sides);
        
        GLfloat x = 0, y = 0, z = 0;
        
        // circle around y axis
        for(int i = 0; i <= sides; i++){
            
            GLuint k = 0;
            
            for(GLfloat h = 0; h <= 2*r; h += dh){
                
                y = h - r;
                
                // use sphere function
                x = cos(angle)*(sqrt(r*r - (h - r)*(h - r)));
                z = -sin(angle)*(sqrt(r*r - (h - r)*(h - r)));
                
                Vertex v;
                v.m_Position = glm::vec3(x,y,z);
                v.m_TexCoords = glm::vec2(1/(GLfloat)(sides)*i, 1/(GLfloat)(sides)*k);
                v.m_Normal = glm::normalize(glm::vec3(x,y,z));
                vertices.push_back(v);
                k++;
            }
            angle += dAngle;
        }
        
        return vertices;
    }
    
    /**
     * Creates a ground based on height map
     *
     * @return created plane
     */
    template <class V>
    static Mesh<V> createGround(const GLchar* filePath){
        
        GLint w, h;
        std::vector<V> vertices = getGroundData<V>(filePath, &w, &h);
        std::vector<GLuint> indices = getPlaneIndices<GLuint>(w, h);
        
        Mesh<V>::calculateNormals(vertices, indices);
        
        Mesh<V> mesh = Mesh<V>(vertices, indices, GL_STATIC_DRAW);
        return mesh;
        
    }
    
    /**
     * Creates a cylinder
     *
     * @param number Number of faces
     * @param height Height of the cylinder
     * @param radius Radius of the cylinder
     */
    template <class V>
    static Mesh<V> createCylinder(GLuint number, GLfloat height, GLfloat radius){
        
        std::vector<V> vertices = getCylinderData<V>(number, height, radius);
        std::vector<GLuint> indices = getCylinderIndices<GLuint>(number);
        Mesh<V> mesh = Mesh<V>(vertices, indices, GL_STATIC_DRAW);
        return mesh;
    }
    
    /**
     * Creates a sphere
     *
     * @param sides Number of faces per dimension
     * @param radius Radius of the sphere
     * @return sphere mesh
     */
    
    template <class V>
    static Mesh<V> createSphere(GLuint sides, GLfloat radius){
        
        std::vector<V> vertices = sphere<V>(sides, radius);
        std::vector<GLuint> indices = getPlaneIndices<GLuint>(sides, sides);
        Mesh<V>::calculateNormals(vertices, indices);
        return Mesh<V>(vertices, indices, GL_STATIC_DRAW);
    }


}


#endif /* GeometryGenerator_h */
