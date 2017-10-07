//
//  Application.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Application.h"
#include "InputManager.h"
#include "Time.h"
#include "ConstantBuffers.hpp"

namespace Fox {
    
    GLfloat Time::deltaTime = 0;
    Renderer Application::m_Renderer;
    
/**
 * Quits this application
 */
    void Application::quit() {
        m_Quit = true;
    }
    
    void Application::startApplication(){
        
        Uint32 time = SDL_GetTicks();
        
        // update until the application is closed
        while (!m_Quit)
        {
            Time::deltaTime = 0.001f*(SDL_GetTicks() - time);
            time = SDL_GetTicks();
        
            // handle input
            m_InputManager->handleInput(this);
            
            Update();
            
            // draw the entire scene
            drawScene();
            // swap buffers
            m_glContext->swapBuffers();
            
            // wait for next frame
            SDL_Delay(10);
            
        }
        
    }
    
    void Application::Update() {
        m_Scene.Update();
    }
    
    void Application::drawScene(){
        // clear the screen at first
        clearScreen();
        
        // draw all render contexts
        for(int i = 0; i < m_glContext->getNumberOfRenderContexts(); i++, m_glContext->nextRenderContext()) {
            
            // draw the scene to depth buffer
      //      m_ShadowMap->renderToDepthBuffer(m_glContext);
        //    renderSceneToDepthBuffer();
            // switch back to back buffer
       //     m_ShadowMap->switchToBackBuffer();
            
            // Reset viewport
      //      glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
       //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       //     m_ShadowMap->visualizeDepthBuffer(m_glContext);
         
            m_glContext->setViewPort();
            
            renderScene();
        }
        
        
        
    }
    
    void Application::renderSceneToDepthBuffer(){
    }
    
    void Application::renderScene(){
    
        Renderer& renderer = GetRenderer();
    
        renderer.Begin(m_glContext);
        renderer.Render(m_Scene);
     //   renderer.FillScreenWithTexture();
        renderer.End();
    }
    
    void Application::clearScreen(){
        glClearColor(0.3f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
