//
//  Model.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/13/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Model.h"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Fox {
    
    void Model::loadModel(std::string path, GLboolean bumpMapping){
    
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals |aiProcess_CalcTangentSpace);
        
        // check if evertything is loaded properly
        if(!scene) {
            std::cout << "No scene. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }
        
 /**      if(scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE) {
            std::cout << "Scene incomplete. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }*/
        if(!scene->mRootNode){
            std::cout << "No root node. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }
        
        this->directory = path.substr(0, path.find_last_of('/'));
        
        GameObject* root = new GameObject;
        aiMatrix4x4 identity = aiMatrix4x4();
        
        // start processing from root node
        processNode(scene->mRootNode, root, identity, scene, bumpMapping);
        
        m_Root = root;
    }
    
    void Model::processNode(aiNode* node,  GameObject* sceneNode, aiMatrix4x4& accTransform, const aiScene* scene, GLboolean bumpMapping){
        
        MeshBase* meshBase;
        aiMatrix4x4 transform;
        
        GameObject* parent;
        
        glm::mat4 mat;
        toMat4(&node->mTransformation, mat);
        
            
        GameObject* newObject = new GameObject;
        
        newObject->m_Name = std::string(node->mName.C_Str());
        
     //   newObject->m_Name = node->mn;
        sceneNode->AddChild(newObject);
            
        // process all nodes meshes
        for(GLuint i = 0; i < node->mNumMeshes; i++){
        
            aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];
            
            // if no bump mapping, load normal mesh, otherwise mesh with normal map
            if(!bumpMapping) {
                Mesh<Vertex>* mesh = processMesh(aMesh, scene);
                meshBase = mesh;
            } else {
                Mesh<VertexPNTTB>* bumpedMesh = processBumpedMesh(aMesh, scene);
                meshBase = bumpedMesh;
            }
            //     m_Meshes.push_back(meshBase);
        
            // just set the mesh (assume there is only one mesh per node)
            
            MeshRenderer* mr = newObject->GetComponent<MeshRenderer>();
            
            mr->m_Mesh = static_cast<Mesh<VertexPNTTB>*>(meshBase);
            mr->m_Mesh->computeBoundingSphere(mr->m_Mesh->m_Vertices);
            mr->m_Layer = MeshRenderer::Layer::OPAQUE;
        }
            
        parent = newObject;
        
        Transform* trans = parent->GetComponent<Transform>();
        trans->m_Model = mat;
        
        glm::mat4 transformation; // your transformation matrix.
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        
        glm::decompose(mat, scale, rotation, translation, skew, perspective);
        glm::vec3 rot = glm::eulerAngles(rotation);
        
        trans->m_Position = translation;
        trans->m_Scale = scale;
        trans->m_EulerAngles = rot;
        trans->m_Rotation = rotation;
        
        // process children
        for(GLuint i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], parent, transform, scene, bumpMapping);
        }
    }
    
    Mesh<Vertex>* Model::processMesh(aiMesh* mesh, const aiScene* scene){
        
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        
        // processs vertices
        for(GLuint i = 0; i < mesh->mNumVertices; i++) {
            
            Vertex vertex;
            
            // process position
            glm::vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            vertex.m_Position = position;
            
            
            // process normals
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.m_Normal = normal;
           
            
            // does the mesh contain textures coordinates
            if(mesh->mTextureCoords[0]) {
                // process texture coordinates
                glm::vec2 uvs;
                uvs.x = mesh->mTextureCoords[0][i].x;
                uvs.y = mesh->mTextureCoords[0][i].y;
                vertex.m_TexCoords = uvs;
                
            }
            else {
                
                vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            vertices.push_back(vertex);
        }
        
        // process indices
        for(GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            
            for(GLuint j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
                
            }
        }
        
        Mesh<Vertex>* m = new Mesh<Vertex>(vertices, indices, GL_STATIC_DRAW);
        
       
        // process material
        if(mesh->mMaterialIndex >= 0) {
            
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Albedo);
            
            // add only first diffuse map
            if(diffuseMaps.size() > 0)
            m->addTexture(diffuseMaps[0]);
            
            std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Metallic);
            
            // add only first specular map
            if(specularMaps.size() > 0)
                m->addTexture(specularMaps[0]);
            
            std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Normal);
       //     std::cout << normalMaps.size() << std::endl;
            // add only first normal map
            if(normalMaps.size() > 0){
                m->addTexture(normalMaps[0]);
            }
    
        }
        
        return m;
    }
    
    Mesh<VertexPNTTB>* Model::processBumpedMesh(aiMesh* mesh, const aiScene* scene) {
    
        std::vector<VertexPNTTB> vertices;
        std::vector<GLuint> indices;
        
        // processs vertices
        for(GLuint i = 0; i < mesh->mNumVertices; i++) {
            
            VertexPNTTB vertex;
            
            // process position
            glm::vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            vertex.m_Position = position;
            
            
            // process normals
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.m_Normal = normal;
            
            
            // does the mesh contain textures coordinates
            if(mesh->mTextureCoords[0]) {
                // process texture coordinates
                glm::vec2 uvs;
                uvs.x = mesh->mTextureCoords[0][i].x;
                uvs.y = mesh->mTextureCoords[0][i].y;
                vertex.m_TexCoords = uvs;
                
            }
            else {
                
                vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            // process tangents
            glm::vec3 tangent;
            tangent.x = mesh->mTangents[i].x;
            tangent.y = mesh->mTangents[i].y;
            tangent.z = mesh->mTangents[i].z;
            vertex.m_Tangent = tangent;
            
            // processs bitangents
            glm::vec3 bitangent;
            bitangent.x = mesh->mBitangents[i].x;
            bitangent.y = mesh->mBitangents[i].y;
            bitangent.z = mesh->mBitangents[i].z;
            vertex.m_Bitangent = bitangent;
            
            vertices.push_back(vertex);
        }
        
        // process indices
        for(GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            
            for(GLuint j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
                
            }
        }
        
        Mesh<VertexPNTTB>* m = new Mesh<VertexPNTTB>(vertices, indices, GL_STATIC_DRAW);
        
        
        // process material
        if(mesh->mMaterialIndex >= 0) {
            
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Albedo);
            
            // add only first diffuse map
            if(diffuseMaps.size() > 0)
                m->addTexture(diffuseMaps[0]);
            
            std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Metallic);
            
            // add only first specular map
            if(specularMaps.size() > 0)
                m->addTexture(specularMaps[0]);
            
            std::vector<Texture*> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, Texture::Normal);
            //std::cout << normalMaps.size() << std::endl;
            if(normalMaps.size() > 0){
                m->addTexture(normalMaps[0]);
            }
            
        }
        
        return m;
    }
    
    
    
    std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                      Texture::TextureType texType) {
    
        std::vector<Texture*> textures;
        
        for(GLuint i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString str;
            material->GetTexture(type, i, &str);
            
            std::string filename = std::string(str.C_Str());
            filename = directory + '/' + filename;
            
            const GLchar* filePath = filename.c_str();
            
            Texture* texture = nullptr;
            TextureManager* textureManager = TextureManager::Instance();
            texture = textureManager->loadTexture(filePath, texType);
            
            if(texture->m_Type != texType){
                
                std::cout << filePath << " " << texType << std::endl;
                std::cout << texture->m_Type << std::endl;
                std::cout << "----------" << std::endl;
                texture->m_Type = texType;
               // std::cout << "error " << texType << " " << filePath << std::endl;
                
                //textureManager->printAll();
            }
            
            textures.push_back(texture);
        }
        return textures;
    }
}
