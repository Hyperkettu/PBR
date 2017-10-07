//
//  Scene.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/18/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Scene.hpp"
#include "TextureManager.h"
#include "Model.h"
#include "Time.h"

namespace Fox {
    
    void Scene::InitScene() {
        
        TextureManager* textureManager = TextureManager::Instance();
        
        Mesh<VertexPNTTB>* sphere = createGeosphere<VertexPNTTB>();
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2-albedo.png"));
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2-normal.png"));
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2_Ambient_Occlusion.png"));
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2-metalness.png"));
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2-roughness.png"));
        sphere->addTexture(textureManager->getTexture("Models/ground/Pocked-stone2-height.png"));
        sphere->computeBoundingSphere(sphere->m_Vertices);
        
        GameObject* sphereObject = new GameObject;
        sphereObject->GetComponent<MeshRenderer>()->m_Mesh = sphere;
        sphereObject->GetComponent<MeshRenderer>()->m_Layer = MeshRenderer::Layer::OPAQUE;
        sphereObject->GetComponent<Transform>()->m_Scale = glm::vec3(5.0f, 5.0f, 5.0f);
        sphereObject->m_Name = "sphereNear";
        AddObject(sphereObject);
        
        
        /**
         textureManager->loadTexture("Models/bark/bark1-albedo.png", Texture::Albedo);
         textureManager->loadTexture("Models/bark/bark1-normal.png", Texture::Normal);
         textureManager->loadTexture("Models/bark/bark1-ao.png", Texture::Ambient_Occlusion);
         textureManager->loadTexture("Models/bark/bark1-metallic.png", Texture::Metallic);
         textureManager->loadTexture("Models/bark/bark1-roughness.png", Texture::Roughness);
         textureManager->loadTexture("Models/bark/bark1-height.png", Texture::Displacement);
         */
        
        Mesh<VertexPNTTB>* sphere2 = createGeosphere<VertexPNTTB>();
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-albedo.png"));
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-normal.png"));
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-ao.png"));
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-metallic.png"));
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-roughness.png"));
        sphere2->addTexture(textureManager->getTexture("Models/bark/bark1-height.png"));
        sphere2->computeBoundingSphere(sphere2->m_Vertices);
        
        GameObject* sphereObject2 = new GameObject;
        sphereObject2->GetComponent<MeshRenderer>()->m_Mesh = sphere2;
        sphereObject2->GetComponent<MeshRenderer>()->m_Layer = MeshRenderer::Layer::OPAQUE;
        sphereObject2->GetComponent<Transform>()->m_Position.z -= 15.0f;
        sphereObject2->GetComponent<Transform>()->m_Scale = glm::vec3(5.0f, 5.0f, 5.0f);
        sphereObject2->m_Name = "sphereFar";
        AddObject(sphereObject2);
        
     //   sphereObject->AddChild(sphereObject2);*/
        
        
        Mesh<VertexPNTTB>* sphere3 = createGeosphere<VertexPNTTB>();
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_albedo.png"));
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_normal.png"));
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_ao.png"));
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_displacement.png"));
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_roughness.png"));
        sphere3->addTexture(textureManager->getTexture("Models/copper/Copper-scuffed_metallic.png"));
        sphere3->computeBoundingSphere(sphere3->m_Vertices);
        
        GameObject* sphereObject3 = new GameObject;
        sphereObject3->GetComponent<MeshRenderer>()->m_Mesh = sphere3;
        sphereObject3->GetComponent<MeshRenderer>()->m_Layer = MeshRenderer::Layer::OPAQUE;
        sphereObject3->GetComponent<Transform>()->m_Position.z -= 30.0f;
        sphereObject3->GetComponent<Transform>()->m_Scale = glm::vec3(5.0f, 5.0f, 5.0f);
        sphereObject3->m_Name = "sphereFar";
        AddObject(sphereObject3);
        
        Mesh<VertexPNTTB>* sphere4 = createGeosphere<VertexPNTTB>();
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-albedo.png"));
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-normal.png"));
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-ao.png"));
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-metallic.png"));
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-roughness.png"));
        sphere4->addTexture(textureManager->getTexture("Models/fabric/old-textured-fabric-displacement.png"));
        sphere4->computeBoundingSphere(sphere4->m_Vertices);
        
        GameObject* sphereObject4 = new GameObject;
        sphereObject4->GetComponent<MeshRenderer>()->m_Mesh = sphere4;
        sphereObject4->GetComponent<MeshRenderer>()->m_Layer = MeshRenderer::Layer::OPAQUE;
        sphereObject4->GetComponent<Transform>()->m_Position.z -= 45.0f;
        sphereObject4->GetComponent<Transform>()->m_Scale = glm::vec3(5.0f, 5.0f, 5.0f);
        sphereObject4->m_Name = "sphereFar";
        AddObject(sphereObject4);
        
        textureManager->loadTexture("Models/fabric/old-textured-fabric-albedo.png", Texture::Albedo);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-normal.png", Texture::Normal);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-ao.png", Texture::Ambient_Occlusion);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-metallic.png", Texture::Metallic);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-roughness.png", Texture::Roughness);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-displacement.png", Texture::Displacement);
        
        
       /* Model nano = Model("Models/nanosuit/nanosuit.obj", true);
        GameObject* nanoObject = nano.GetGameObject();
        AddObject(nanoObject);
        */
        
        // init skybox
        std::vector<const GLchar*> faces;
        
        faces.push_back("Textures/Skybox/right.jpg");
        faces.push_back("Textures/Skybox/left.jpg");
        faces.push_back("Textures/Skybox/top.jpg");
        faces.push_back("Textures/Skybox/bottom.jpg");
        faces.push_back("Textures/Skybox/back.jpg");
        faces.push_back("Textures/Skybox/front.jpg");
        
    /*    faces.push_back("Textures/starfield/right.tga");
        faces.push_back("Textures/starfield/left.tga");
        faces.push_back("Textures/starfield/top.tga");
        faces.push_back("Textures/starfield/bottom.tga");
        faces.push_back("Textures/starfield/back.tga");
        faces.push_back("Textures/starfield/front.tga");*/
        
        
        m_Skybox = Skybox(faces);
        
        // define sun
        m_Sun.m_Ambient = glm::vec3(0.0f, 0.0f, 0.05f);
        m_Sun.m_Diffuse = glm::vec3(1.0f, 0.95f, 0.65f);
        m_Sun.m_Specular = glm::vec3(1.0f, 1.0f, 1.0f);
        m_Sun.m_Direction = glm::vec3(2.0f, -3.0f, 3.0f);
       // m_Sun.m_Color = glm::vec3(300.0f, 300.0f ,300.0f);
        m_Sun.m_Color = glm::vec3(10.0f, 10.0f, 10.0f);
        
    }
    
    void Scene::Update() {
    
      //  m_SunAngle += 5.0f * Time::deltaTime;
      //  glm::mat4 mat = glm::rotate(glm::mat4(1.0f), m_SunAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        
    //   m_Sun.m_Direction = glm::mat3(mat) * glm::vec3(1.0f, 0.0f, 0.0f);
        
        sceneGraph.TraverseGraph(sceneGraph.update);
    }
    
    
}

