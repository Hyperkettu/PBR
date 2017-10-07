//
//  Math.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 9/17/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Math_h
#define Math_h

#include <cmath>

static float angleFromXY(float x, float y){
    
    float theta = 0.0f;
    float PI = 3.14159265358f;
    
    if(x >= 0.0f){
        
        theta = atanf(y/x);
        
        if(theta < 0.0f){
            theta += 2 * PI;
        }
        
    } else
        theta = atanf(y/x) + PI;
    
    return theta;
}


#endif /* Math_h */
