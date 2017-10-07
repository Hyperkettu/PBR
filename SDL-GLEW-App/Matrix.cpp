//
//  Matrix.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Matrix.hpp"

namespace Fox {

    glm::mat4 viewport (GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
        GLfloat w2 = static_cast<GLfloat>(width/2.0f);
        GLfloat h2 = static_cast<GLfloat>(height/2.0f);
        return glm::mat4(w2, 0, 0, 0,
                         0, h2, 0, 0,
                         0, 0, 1, 0,
                         x + w2, y + h2, 0, 1);
    }
    
    glm::mat4 toMat4(const glm::mat3& matrix){
        glm::mat4 m;
        
        for(unsigned int i = 0; i < 3; i++)
            for(unsigned int j = 0; j < 3; j++)
                m[j][i] = matrix[j][i];
        
        m[0][3] = 0.0f;
        m[1][3] = 0.0f;
        m[2][3] = 0.0f;
        m[3][3] = 1.0f;
        
        return m;
    }
    
    glm::mat3 toMat3(const glm::mat4& matrix){
        glm::mat3 m;
        
        for(unsigned int i = 0; i < 3; i++)
            for(unsigned int j = 0; j < 3; j++)
                m[j][i] = matrix[j][i];
        
        return m;
    }
    
    
    void printMatrix(const glm::mat4& m){
        
        std::cout << std::endl << "matrix:" << std::endl;
        
        for(int i = 0; i < 4; i++){
            for(int j = 0 ; j < 4; j++)
                std::cout << m[j][i] << " ";
            std::cout << std::endl;
        }
    }
    
    
    void toMat4(const aiMatrix4x4 *from, glm::mat4 &to) {
        to[0][0] = from->a1; to[1][0] = from->a2;
        to[2][0] = from->a3; to[3][0] = from->a4;
        to[0][1] = from->b1; to[1][1] = from->b2;
        to[2][1] = from->b3; to[3][1] = from->b4;
        to[0][2] = from->c1; to[1][2] = from->c2;
        to[2][2] = from->c3; to[3][2] = from->c4;
        to[0][3] = from->d1; to[1][3] = from->d2;
        to[2][3] = from->d3; to[3][3] = from->d4;
    }
    
    
}
