//
//  InputManager.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "InputManager.h"

namespace Fox {

    void InputManager::handleInput(Application *app) {
    
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
        
        }
    
    }
}
