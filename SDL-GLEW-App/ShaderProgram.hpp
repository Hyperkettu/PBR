//
//  ShaderProgram.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <GL/glew.h>

#include <iostream>
#include "glm/glm.hpp"

namespace Fox {
    
    enum Shader {
    
        BLINN_PHONG = 0,
        PBR,
        PBR_DISPLACEMENT,
        SKYBOX,
        IRRADIANCE,
        PREFILTERED_ENV_MAP,
        INTEGRATION_BRDF,
        FILL_SCREEN,
        
        NUM_SHADERS
        
    };

class ShaderProgram {
public:
    
    /**
     * Creates a shader program from given vertex and fragment shader sources
     *
     * @param vertexShaderSource Source for vertex shader
     * @param vfSize Vertex file size
     * @param fragmentShaderSource Source for fragment shader
     * @param ffSize Fragment shader file size
     */
    ShaderProgram(const GLchar* vertexShaderSource, GLint vfSize, const GLchar* fragmentShaderSource, GLint ffSize);
    
    /**
     * Creates a shader program from given vertex, geometry and fragment shader sources
     *
     * @param vertexShaderSource Source for vertex shader
     * @param vfSize Vertex file size
     * @param geometryShaderSource Source for geometry shader
     * @param gfSize Geometry shader file size
     * @param fragmentShaderSource Source for fragment shader
     * @param ffSize Fragment shader file size
     */
    ShaderProgram(const GLchar* vertexShaderSource, GLint vfSize, const GLchar* geometryShaderSource, GLint gfSize, const GLchar* fragmentShaderSource, GLint ffSize);
    
    ShaderProgram(const GLchar* vsSource, GLint vsSize, const GLchar* tcsSource, GLint tcsSize, const GLchar* tesSource, GLint tesSize, const GLchar* fsSource, GLint fsSize);
    
    /**
     * Destroys this shader program
     */
    ~ShaderProgram(){
        glDeleteProgram(m_Id);
    }
    
    /**
     * Creates an OpenGL shader of given type from the source
     * 
     * @param type Type of the shader
     * @param shaderSource Source for the shader
     * @param fSize File size
     * @return shader id
     */
    GLuint createGLShader(GLenum type, const GLchar* shaderSource, GLint fSize);
    
    
    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_Id, name.c_str()), (int)value);
    }
    
    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
    }
    
    void setSampler(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_Id, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_Id, name.c_str()), value);
    }
    
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
    }
    
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_Id, name.c_str()), x, y);
    }
    
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
    }
    
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_Id, name.c_str()), x, y, z);
    }
    
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_Id, name.c_str()), 1, &value[0]);
    }
    
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_Id, name.c_str()), x, y, z, w);
    }
    
    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    
    
    
    GLuint m_Id; // id of the shader program
    
private:
    
    GLuint m_VertexShader, m_GeometryShader, m_FragmentShader; ///< ids for shader types
    GLuint m_TessControlShader, m_TessEvaluationShader;
};
    
}

#endif /* ShaderProgram_hpp */
