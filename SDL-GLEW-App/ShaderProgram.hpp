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
#include <unordered_map>

namespace Fox {

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
    
    /**
     * Adds an uniform variable to shader
     *
     * @param name Name of the uniform
     * @return identifier for the uniform
     */
    GLint addUniform(const GLchar* name){
        GLint uniform = glGetUniformLocation(m_Id, name);
        uniforms[name] = uniform;
        return uniform;
    }
    
    bool hasUniform(const GLchar* uniformName){
        return uniforms.find(uniformName) != uniforms.end();
    }
    
    /**
     * Returns a location of an uniform
     *
     * @param uniformName Name for uniform
     * @return GLint location of the uniform
     */
    inline GLint getLocation(const GLchar* uniformName) const{
        return uniforms.find(uniformName)->second;
    }
    
    GLuint m_Id; // id of the shader program
    
private:
    
    GLuint m_VertexShader, m_GeometryShader, m_FragmentShader; ///< ids for all shader types
    std::unordered_map<const GLchar*, GLint> uniforms; ///< uniform map
};
    
}

#endif /* ShaderProgram_hpp */
