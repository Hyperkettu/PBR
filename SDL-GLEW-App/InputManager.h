//
//  InputManager.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef InputManager_h
#define InputManager_h

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include "Application.h"

namespace Fox {
    
    class Application;
    
class InputManager {

public:
    
    InputManager() : m_MouseX(0), m_MouseY(0) {
    }
    
    /**
     * Handles SDL input
     * 
     * @param app Application whose input is handled
     */
    void handleInput(Application *app);
    
    int m_MouseX, m_MouseY; // mouse coordinates as pixels
    
};

}

#endif /* InputManager_h */
