//
//  GLContext.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef GLContext_h
#define GLContext_h

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <SDL2/SDL_opengl.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ResourceManager.h"
#include "ShaderProgram.hpp"
#include "RenderContext.h"
#include "ShaderManager.hpp"
#include "RenderTargetState.hpp"

/**
 * OpenGL context
 */

namespace Fox {

class GLContext {

public:
    
    GLContext(){}
    
    /**
     * Creates an OpenGL context for the window
     *
     * @param window Window to render
     */
    GLContext(SDL_Window* window, const ResourceManager& resourceManager) : m_ResourceManager(resourceManager){
    
        // create OpenGL context
        m_Context = SDL_GL_CreateContext(window);
        
        glewExperimental = true;
        
        // initialize GLEW
        if(GLEW_OK != glewInit()) {
            std::cout << "Failed to initialize GLEW" << std::endl;
            
        }
        
        m_CurrentShader = Shader::BLINN_PHONG;
        m_Window = window;
        m_CurrentRenderContext = 0;
    }
    
    /**
     * Destroy OpenGL context
     */
    ~GLContext(){
        
        // remove all shaders
     /*   for(ShaderProgram* p : m_Shaders) {
            delete p;
        }*/
        
   //     m_Shaders.clear();
        
        
       // delete m_Window;
        
        // delete OpenGL context
        SDL_GL_DeleteContext(m_Context);
    
    }
    
    /**
     * Adds a render context to this GL context
     *
     */
    void addRenderContext(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat fov, GLfloat near, GLfloat far) {
    
      //  m_RenderContext = RenderContext(x, y, width, height, fov, near, far);
        m_RenderContexts.push_back(RenderContext(x, y, width, height, fov, near, far));
    }
    
    /**
     * Sends projection matrix uniform of current render context to shader
     *
     * @param projectionName Name of the uniform in shader
     */
    void setProjectionUniform(const GLchar* projectionName){
        ShaderManager::GetShader((Shader)m_CurrentShader)->setMat4(projectionName, m_RenderContexts[m_CurrentRenderContext].m_Projection);
    }
    
    
    /**
     * Sends view matrix uniform of current render context to shader
     *
     * @param viewName Name of the uniform in shader
     */
    void setViewUniform(const GLchar* viewName){
        ShaderManager::GetShader((Shader)m_CurrentShader)->setMat4(viewName, m_RenderContexts[m_CurrentRenderContext].m_Camera.view());
    }
    
    void setViewUniformForSkybox(const GLchar* viewName){
        // remove translation component from view matrix
        ShaderManager::GetShader((Shader)m_CurrentShader)->setMat4(viewName, glm::mat4(glm::mat3(m_RenderContexts[m_CurrentRenderContext].m_Camera.view())));
    }
    
    void setCameraPosition(const GLchar* uniformName) {
        ShaderManager::GetShader((Shader)m_CurrentShader)->setVec3(uniformName, m_RenderContexts[m_CurrentRenderContext].m_Camera.m_Position);
    }
    
    /**
     * Sets view port of the current render context
     */
    void setViewPort(){
        RenderContext& rc = m_RenderContexts[m_CurrentRenderContext];
        glViewport(rc.m_ViewPortX, rc.m_ViewPortY, rc.m_ViewPortWidth, rc.m_ViewPortHeight);
    }
    
    /**
     * Swaps render buffers
     */
    
    inline void swapBuffers(){
        SDL_GL_SwapWindow(m_Window);
    }
    
    /**
     * Adds a shader program to shaders using given source files
     *
     * @param vertexShaderFile File containing vertex shader source
     * @param fragmentShaderFile File containing fragment shader source
     */
    void addShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
    
    /**
     * Use a given shader for rendering
     */
    void useShader() {
        m_CurrentShader = Shader::BLINN_PHONG;
        glUseProgram(ShaderManager::GetShader(m_CurrentShader)->m_Id);
    }
    
    void useShader(Shader type){
        m_CurrentShader = type;
        glUseProgram(ShaderManager::GetShader(type)->m_Id);
    }
    
    /**
     * Specify the value of a uniform variable for the current shader. Binds a texture to a texture unit with given name
     *
     * @param texture Texture id to be bound
     * @param textureUnit Texture unit to bind
     * @param sampler2Dname Name of the sampler uniform in shader
     * @param value What index should be bound
     */
    void bindTexture(GLuint texture, GLenum textureUnit, const GLchar* sampler2Dname, GLint value){
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture);
        ShaderManager::GetShader((Shader) m_CurrentShader)->setSampler(sampler2Dname, value);
        
    }
    
    void SetSamplerTexture(Texture* texture, const GLchar* sampler2Dname, GLint value){
        glActiveTexture(GL_TEXTURE0 + value);
        
        if(texture->m_IsCubeMap)
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture->m_Id);
        else
            glBindTexture(GL_TEXTURE_2D, texture->m_Id);
        ShaderManager::GetShader((Shader) m_CurrentShader)->setSampler(sampler2Dname, value);
    }
    
  /*  void setCurrentShader(GLint index){
        m_CurrentShader = index;
    }*/
    
    /**
     * Returns current render context
     *
     * @return render context
     */
    inline RenderContext& getCurrentRenderContext(){
        return m_RenderContexts[m_CurrentRenderContext];
    }
    
    /**
     * Returns render context of certain index 
     * 
     * @param index Index of the render context
     * @return render context
     */
    inline RenderContext& getRenderContextOfIndex(GLuint index){
        return m_RenderContexts[index];
    }
    
    /**
     * Switches current render context
     */
    inline void nextRenderContext(){
        m_CurrentRenderContext = (m_CurrentRenderContext + 1) % m_RenderContexts.size();
    }
    
    /**
     * Returns number of render contexts
     */
    inline GLint getNumberOfRenderContexts() {
        return m_RenderContexts.size();
    }
    
    /**
     * Returns SDL Window that is used for rendering
     */
    inline SDL_Window* getWindow(){
        return m_Window;
    }
    
    inline ShaderProgram* getCurrentShader(){
        return ShaderManager::GetShader((Shader)m_CurrentShader);
    }
    
    void RenderTargetBegin(RenderTargetState& rts);
    void RenderTargetEnd();
    
    
private:
    
    SDL_GLContext m_Context; ///< SDL GL context
    SDL_Window* m_Window; // window for rendering
    
    RenderContext m_RenderContext; ///< render context
    GLuint m_CurrentRenderContext; ///< index of current render context
    std::vector<RenderContext> m_RenderContexts;
    
    ResourceManager m_ResourceManager; ///< resource managing
    
   // std::vector<ShaderProgram*> m_Shaders; ///< shaders
    Shader m_CurrentShader; ///< current shader index

};

}
    
#endif /* GLContext_h */
