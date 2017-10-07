//
//  RenderTargetState.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "RenderTargetState.hpp"

namespace Fox {
    
    std::stack<RenderTargetState> RenderTargetState::renderTargetStack;
}
