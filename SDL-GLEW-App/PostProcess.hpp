//
//  PostProcess.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/20/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef PostProcess_hpp
#define PostProcess_hpp

#include <GL/glew.h>

namespace Fox {

class PostProcess {

public:
    
    PostProcess() {
    
        m_QuadVAO = 0;
    }
    
    void RenderToScreen(GLuint texture);

    
    void RenderQuad();
    
private:
    
    GLuint m_QuadVAO, m_QuadVBO;
};

}
    
#endif /* PostProcess_hpp */
