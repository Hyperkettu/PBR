//
//  Model.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/13/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "GLContext.h"
#include "Mesh.h"
#include "TextureManager.h"
#include "Vector.h"
#include "GameObject.hpp"

namespace Fox {
    
    /**
     * Model consists of several meshes
     */
    class Model {
    
    public:
        Model(){}
        
        
        /**
         * Creates a model from data path
         *
         * @param path Path to the model data
         */
        Model(GLchar* path, GLboolean bumpMapping = false){
            loadModel(path, bumpMapping);
        }
        
        /**
         * Draws this model 
         *
         * @param gl GLContext
         */
        void draw(GLContext* gl) {
            // draw all meshes
            for(GLuint i = 0; i < m_Meshes.size(); i++){
                m_Meshes[i]->draw(gl);
            }
        }
        
        std::vector<MeshBase*>& GetMeshes() {
            return m_Meshes;
        }
        
        void ComputeBoundingSpheres(){
            for(GLuint i = 0; i < m_Meshes.size(); i++){
                Mesh<VertexPNTTB>* mesh = static_cast<Mesh<VertexPNTTB>*>(m_Meshes[i]);
                m_Meshes[i]->computeBoundingSphere(mesh->m_Vertices);
            }
        }
        
        void drawWireframe(GLContext* gl){
            for(GLuint i = 0; i < m_Meshes.size(); i++){
                m_Meshes[i]->drawWireframe(gl);
            }
        }
        
        void addTexture(Texture* texture){
            for(GLuint i = 0; i < m_Meshes.size();i++){
                m_Meshes[i]->addTexture(texture);
            }
        }
        
        inline GameObject* GetGameObject(){
            return m_Root;
        }
        
    private:
        
        /**
         * Loads the model from path
         *
         * @param path Path of the model
         */
        void loadModel(std::string path, GLboolean bumpMapping = false);
        
        /**
         * Processes an aiNode
         *
         * @param node aiNode to be processed
         * @param scene aiScene
         */
        void processNode(aiNode* node, GameObject* sceneNode, aiMatrix4x4& accTransform, const aiScene* scene, GLboolean bumpMapping = false);
        
        /**
         * Processes an aiMesh to Mesh<Vertex>
         *
         * @param mesh aiMesh to be processed
         * @param scene aiScene
         * @return Mesh<Vertex>
         */
        Mesh<Vertex>* processMesh(aiMesh* mesh, const aiScene* scene);
        
        Mesh<VertexPNTTB>* processBumpedMesh(aiMesh* mesh, const aiScene* scene);
        
        /**
         * Loads the materials from aiMaterial of given type
         *
         * @param material aiMaterial to be processed
         * @param type Type of the textures
         */
        std::vector<Texture*> loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                   Texture::TextureType texType);
    
        std::string directory; ///< model file directory
        std::vector<MeshBase*> m_Meshes; ///< all meshes of this model
        GameObject* m_Root;
        
    };
}

#endif /* Model_h */
