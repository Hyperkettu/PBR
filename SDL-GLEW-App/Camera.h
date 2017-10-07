//
//  Camera.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/28/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <GL/glew.h>

#include "glm/glm.hpp"'
#include "glm/gtc/matrix_transform.hpp"

namespace Fox {

class Camera {

public:
    
    const GLfloat YAW        = -90.0f;
    const GLfloat PITCH      =  0.0f;
    const GLfloat SENSITIVTY =  0.25f;
    
    /**
     * Creates a camera in position oriented in the front direction with up vector
     *
     * @param position Position of the camera
     * @param front Front of the camera
     * @param up Up of the camera
     */
    Camera(const glm::vec3 position = glm::vec3(0.0f, 5.0f, 10.0f), const glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)) : m_Position(position), m_Front(front), m_Up(up) {
        
        m_MovementSpeed = 20.0f;
        
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        
       // m_Front = -position;
        glm::normalize(m_Front);
        
        m_Right = glm::cross(m_Front, m_WorldUp);
        
        glm::normalize(m_Right);
        
        m_Up = glm::cross(m_Right, m_Front);
        
        m_Yaw = YAW;
        m_Pitch = PITCH;
        
        m_MouseSensitivity = SENSITIVTY;
        
        updateBaseVectors();
    }
    
    
    /**
     * Set camera position
     */
    void setPosition(const glm::vec3& newPosition);
    
    /**
     * Constructs a view matrix
     */
    glm::mat4 view() {
        
        return glm::lookAt(this->m_Position, this->m_Position + this->m_Front, this->m_Up);
    }
    
    /**
     * Moves this camera forward
     * 
     * @param deltaTime Delta in time
     */
    inline void moveForward(float deltaTime){
        m_Position += m_Front * m_MovementSpeed * deltaTime;
    }
    
    /**
     * Moves this camera backward
     *
     * @param deltaTime Delta in time
     */
    inline void moveBackward(float deltaTime){
        m_Position -= m_Front * m_MovementSpeed * deltaTime;
    }
    
    /**
     * Moves this camera right
     *
     * @param deltaTime Delta in time
     */
    inline void moveRight(float deltaTime){
        m_Position += m_Right * m_MovementSpeed * deltaTime;
    }
    
    /**
     * Moves this camera left
     *
     * @param deltaTime Delta in time
     */
    inline void moveLeft(float deltaTime){
        m_Position -= m_Right * m_MovementSpeed * deltaTime;
    }
    
    inline void moveForwardZ(float deltaTime){
        m_Position -= glm::vec3(0, 0, 1) * m_MovementSpeed * deltaTime;
    }
    
    inline void moveBackwardZ(float deltaTime){
        m_Position += glm::vec3(0, 0, 1) * m_MovementSpeed * deltaTime;
    }
    
    inline void rotate(int xRel, int yRel){
    
        GLfloat xOffset = xRel;
        GLfloat yOffset = -yRel;
        
        xOffset *= m_MouseSensitivity;
        yOffset *= m_MouseSensitivity;
        
        m_Yaw   += xOffset;
        m_Pitch += yOffset;
        
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
        
        // Update Front, Right and Up Vectors using the updated Eular angles
        updateBaseVectors();
    
    }
    
    void updateBaseVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
    
    glm::vec3 m_Position; ///< camera position
    glm::vec3 m_Front; ///< camera front
    glm::vec3 m_Up; ///< camera up
    glm::vec3 m_Right; ///< camera right
    glm::vec3 m_WorldUp; ///< world up
    
    glm::vec3 m_CenterOfInterest; ///< center of interest
    
    GLfloat m_MovementSpeed; ///< mouse movement speed
    GLfloat m_MouseSensitivity; ///< mouse sensitivity
    GLfloat m_Yaw; ///< camera yaw
    GLfloat m_Pitch; ///< camera pitch

};

}
    
#endif /* Camera_h */
