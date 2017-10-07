//
//  TextureManager.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/6/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "TextureManager.h"

namespace Fox {

TextureManager* TextureManager::m_Singleton = nullptr;

    void TextureManager::LoadTextures() {
    
        TextureManager* textureManager = TextureManager::Instance();
        
   /*     textureManager->loadTexture("Textures/container.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/grassplain.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/brown.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/tree.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/container_specular.png", Texture::Specular);
        textureManager->loadTexture("Textures/white.png", Texture::Specular);
        textureManager->loadTexture("Textures/black.png", Texture::Specular);
        textureManager->loadTexture("Textures/darkgreen.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/red.png", Texture::Diffuse);*/
        textureManager->loadTexture("Models/ground/Pocked-stone2-albedo.png", Texture::Albedo);
        textureManager->loadTexture("Models/ground/Pocked-stone2-normal.png", Texture::Normal);
        textureManager->loadTexture("Models/ground/Pocked-stone2_Ambient_Occlusion.png", Texture::Ambient_Occlusion);
        textureManager->loadTexture("Models/ground/Pocked-stone2-metalness.png", Texture::Metallic);
        textureManager->loadTexture("Models/ground/Pocked-stone2-roughness.png", Texture::Roughness);
        textureManager->loadTexture("Models/ground/Pocked-stone2-height.png", Texture::Displacement);
        
        textureManager->loadTexture("Models/bark/bark1-albedo.png", Texture::Albedo);
        textureManager->loadTexture("Models/bark/bark1-normal.png", Texture::Normal);
        textureManager->loadTexture("Models/bark/bark1-ao.png", Texture::Ambient_Occlusion);
        textureManager->loadTexture("Models/bark/bark1-metallic.png", Texture::Metallic);
        textureManager->loadTexture("Models/bark/bark1-roughness.png", Texture::Roughness);
        textureManager->loadTexture("Models/bark/bark1-height.png", Texture::Displacement);
        
        textureManager->loadTexture("Models/copper/Copper-scuffed_albedo.png", Texture::Albedo);
        textureManager->loadTexture("Models/copper/Copper-scuffed_normal.png", Texture::Normal);
        textureManager->loadTexture("Models/copper/Copper-scuffed_ao.png", Texture::Ambient_Occlusion);
        textureManager->loadTexture("Models/copper/Copper-scuffed_metallic.png", Texture::Metallic);
        textureManager->loadTexture("Models/copper/Copper-scuffed_roughness.png", Texture::Roughness);
        textureManager->loadTexture("Models/copper/Copper-scuffed_displacement.png", Texture::Displacement);
        
        textureManager->loadTexture("Models/fabric/old-textured-fabric-albedo.png", Texture::Albedo);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-normal.png", Texture::Normal);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-ao.png", Texture::Ambient_Occlusion);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-metallic.png", Texture::Metallic);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-roughness.png", Texture::Roughness);
        textureManager->loadTexture("Models/fabric/old-textured-fabric-displacement.png", Texture::Displacement);
        
    }
    
}
