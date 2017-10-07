//
//  InputManager.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Application.h" // application before inputmanager to avoid allocation on incomplete type
#include "InputManager.h"

namespace Fox {

    void InputManager::handleInput(Application *app) {
    
        // get mouse position
        Uint32 mouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);
        
        // if left mouse button is held down
        if(mouseState & SDL_BUTTON(1)){
            app->moveMouseForward();
            // if middle mouse button is held down
        } else if(mouseState & SDL_BUTTON(2)) {
          //  app->SetContainerPositionToMousePosition();
            app->moveMouseBackward();
            // if right mouse button is held down
        } else if(mouseState & SDL_BUTTON(3)){
            app->SetContainerPositionToMousePosition();
        }
        
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        if(keyState[SDL_SCANCODE_A]){
            app->moveCameraLeft();
        }
        
        if(keyState[SDL_SCANCODE_D]){
            app->moveCameraRight();
        }
        
        if(keyState[SDL_SCANCODE_W]){
            app->moveMouseForward();
        }
        
        if(keyState[SDL_SCANCODE_S]){
            app->moveMouseBackward();
        }
        
        SDL_Event windowEvent;
        
        // if there is input
        if(SDL_PollEvent(&windowEvent)){
        
            // close window event
            if (windowEvent.type == SDL_QUIT) {
                app->quit();
            }
            // close window event
            if (windowEvent.type == SDL_KEYUP &&
                windowEvent.key.keysym.sym == SDLK_ESCAPE){
                app->quit();
            }
            
            // if f is pressed, toggle full screen
            if(windowEvent.type == SDL_KEYUP && windowEvent.key.keysym.sym == SDLK_f) {
                app->toggleFullScreen();
            }
            
            if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
                // If the left button was pressed
                if (windowEvent.button.button == SDL_BUTTON_LEFT) {

                }
            }
            
            // mouse moved
            if(windowEvent.type == SDL_MOUSEMOTION) {
                app->GetGLContext()->getCurrentRenderContext().m_Camera.rotate(windowEvent.motion.xrel, windowEvent.motion.yrel);
            }
        }
    
    }
}
