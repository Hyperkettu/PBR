//
//  Texture.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/28/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include <iostream>

#include <GL/glew.h>

#include <SDL2_image/SDL_image.h>

namespace Fox {
  
    class Texture {
        
    public:
        
    /*    enum TextureType
        {
            Diffuse = 0,    // Diffuse color map.
            Specular,       // Specular map
            Alpha,          // Alpha map (green = opacity).
            Displacement,   // Displacement map (green = height).
            Normal,         // Tangent-space normal map.
            Environment,    // Environment map (spherical coordinates).
            
            TextureType_Max
        };*/
        
         enum TextureType
        {
            Albedo = 0,
            Normal,
            Roughness,
            Metallic,
            Ambient_Occlusion,
            Displacement,
            
            TextureType_Max
        };
        
        
        Texture(){
        
            m_IsCubeMap = false;
        }
        
        /**
         * Destroy loaded texture
         */
        ~Texture(){
            //std::cout << "delete " << m_Id << std::endl;
            glDeleteTextures(1, &m_Id);
            
        }
        
        static SDL_Surface* readTexture(const GLchar* filePath){
            
            SDL_Surface* image;
            
            // load image from file
            image = IMG_Load(filePath);
            
            // if image loading failed
            if(!image) {
                std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
            }
            return image;
        }
        
        static void freeTexture(SDL_Surface* image){
            SDL_FreeSurface(image);
        }
        
        /**
         * Loads the texture from file path
         *
         * @param filePath File path
         */
        void load(const GLchar* filePath) {
        
            SDL_Surface* image;
            
            // load image from file
            image = IMG_Load(filePath);
            
            // if image loading failed
            if(!image) {
                std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
            }
            
            // create texture
            glGenTextures(1, &m_Id);
            glBindTexture(GL_TEXTURE_2D, m_Id);
            
            GLint mode = GL_RGB;
            
            if(image->format->BytesPerPixel == 4) {
                mode = GL_RGBA;
            }
            
            // set parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            // note BGRA instead of RGBA
            
            GLint internalFormat = GL_BGR;
            
            if(image->format->BytesPerPixel == 4){
                internalFormat = GL_BGRA;
            }
            
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, //GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
                         internalFormat, GL_UNSIGNED_BYTE, image->pixels);
            
            // generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            
          //  SDL_FreeSurface(image);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        static Texture* CreateRenderTarget(GLint internalFormat, GLenum format, GLuint width, GLuint height) {
        
            Texture* texture = new Texture;
            texture->m_Width = width;
            texture->m_Height = height;
            
            GLuint texId;
            glGenTextures(1, &texId);
            
            glBindTexture(GL_TEXTURE_2D, texId);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_FLOAT, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            texture->m_Id = texId;
            
            return texture;
        
        }
        
        /**
         * Loads this texture from a file path 
         *
         * @param filePath File path to texture file
         */
        Texture(const GLchar* filePath) {
            
            load(filePath);
           // std::cout << "load " << m_Id << std::endl;
        }
        
        GLuint m_Id; ///< texture id
        TextureType m_Type; ///< texture type
        GLuint m_Width, m_Height;
        bool m_IsCubeMap;
    };
}


#endif /* Texture_h */
