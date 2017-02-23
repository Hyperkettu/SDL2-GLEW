//
//  ShaderProgram.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "ShaderProgram.hpp"

namespace Fox
{
    
    ShaderProgram::ShaderProgram(const GLchar* vertexShaderSource, GLint vfSize, const GLchar* fragmentShaderSource, GLint ffSize) {

        // create vertex and fragment shader
        m_VertexShader = createGLShader(GL_VERTEX_SHADER, vertexShaderSource, vfSize);
        m_FragmentShader = createGLShader(GL_FRAGMENT_SHADER, fragmentShaderSource, ffSize);
        
        // create shader program
        m_Id = glCreateProgram();
        
        // attach vertex and fragment shaders
        glAttachShader(m_Id, m_VertexShader);
        glAttachShader(m_Id, m_FragmentShader);
        
        // link shader program
        glLinkProgram(m_Id);
        
        GLint success;
        GLchar infoLog[512];
        
        // check if shader program linking succeeded
        glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
        
        // if not, print out error message
        if(!success) {
            glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
            std::cout << "Shader program linking failed" << std::endl;
        }
        // delete unnecessary shaders
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_FragmentShader);
    
    }
    
    ShaderProgram::ShaderProgram(const GLchar* vertexShaderSource, GLint vfSize, const GLchar* geometryShaderSource, GLint gfSize, const GLchar* fragmentShaderSource, GLint ffSize) {
        
        // create vertex, geometry and fragment shader
        m_VertexShader = createGLShader(GL_VERTEX_SHADER, vertexShaderSource, vfSize);
        m_GeometryShader = createGLShader(GL_GEOMETRY_SHADER, geometryShaderSource, gfSize);
        m_FragmentShader = createGLShader(GL_FRAGMENT_SHADER, fragmentShaderSource, ffSize);
    
        // create shader program
        m_Id = glCreateProgram();
        
        // attach vertex and fragment shaders
        glAttachShader(m_Id, m_VertexShader);
        glAttachShader(m_Id, m_GeometryShader);
        glAttachShader(m_Id, m_FragmentShader);
        
        // link shader program
        glLinkProgram(m_Id);
        
        GLint success;
        GLchar infoLog[512];
        
        // check if shader program linking succeeded
        glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
        
        // if not, print out error message
        if(!success) {
            glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
            std::cout << "Shader program linking failed" << std::endl;
        }
        // delete unnecessary shaders
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_GeometryShader);
        glDeleteShader(m_FragmentShader);
        
        std::cout << "Create program" << std::endl;
    }
    
    GLuint ShaderProgram::createGLShader(GLenum type, const GLchar* shaderSource, GLint fSize) {
    
        GLuint shaderId;
        shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, &shaderSource, &fSize);
        glCompileShader(shaderId);
        
        // check if shader compilation succeeded
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        
        // if not, print out error message
        if(!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "Vertex shader compilation failed\n" << infoLog << std::endl;
        }
        
        return shaderId;
    }
}
