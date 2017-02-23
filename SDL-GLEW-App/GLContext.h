//
//  GLContext.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef GLContext_h
#define GLContext_h

#include <iostream>
#include <vector>

#include <SDL2/SDL.h>

#include <GL/glew.h>

#include <SDL2/SDL_opengl.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "ResourceManager.h"
#include "ShaderProgram.hpp"
#include "RenderContext.h"

/**
 * OpenGL context
 */

namespace Fox {

class GLContext {

public:
    
    GLContext(){}
    
    /**
     * Creates an OpenGL context for the window
     *
     * @param window Window to render
     */
    GLContext(SDL_Window* window, const ResourceManager& resourceManager) : m_ResourceManager(resourceManager){
    
        // create OpenGL context
        m_Context = SDL_GL_CreateContext(window);
        
        glewExperimental = true;
        
        // initialize GLEW
        if(GLEW_OK != glewInit()) {
            std::cout << "Failed to initialize GLEW" << std::endl;
            
        }
        
        m_CurrentShader = 0;
        m_Window = window;
        m_CurrentRenderContext = 0;
    }
    
    /**
     * Destroy OpenGL context
     */
    ~GLContext(){
        
        // remove all shaders
        for(ShaderProgram* p : m_Shaders) {
            delete p;
        }
        
        m_Shaders.clear();
        
        
        // delete OpenGL context
        SDL_GL_DeleteContext(m_Context);
    
    }
    
    /**
     * Adds a render context to this GL context
     *
     */
    void addRenderContext(GLfloat x, GLfloat y, GLfloat width, GLfloat height, GLfloat fov, GLfloat near, GLfloat far) {
    
      //  m_RenderContext = RenderContext(x, y, width, height, fov, near, far);
        m_RenderContexts.push_back(RenderContext(x, y, width, height, fov, near, far));
    }
    
    /**
     * Sends projection matrix uniform of current render context to shader
     *
     * @param projectionName Name of the uniform in shader
     */
    void setProjectionUniform(const GLchar* projectionName){
        setMatrix4fUniform(m_RenderContexts[m_CurrentRenderContext].m_Projection, projectionName);
    }
    
    
    /**
     * Sends view matrix uniform of current render context to shader
     *
     * @param viewName Name of the uniform in shader
     */
    void setViewUniform(const GLchar* viewName){
        setMatrix4fUniform(m_RenderContexts[m_CurrentRenderContext].m_Camera.view(), viewName);
    }
    
    void setViewUniformForSkybox(const GLchar* viewName){
        // remove translation component from view matrix
        setMatrix4fUniform(glm::mat4(glm::mat3(m_RenderContexts[m_CurrentRenderContext].m_Camera.view())), viewName);
    }
    
    void setCameraPosition(const GLchar* uniformName) {
        setVec3(m_RenderContexts[m_CurrentRenderContext].m_Camera.m_Position, uniformName);
    }
    
    /**
     * Sets view port of the current render context
     */
    void setViewPort(){
        RenderContext& rc = m_RenderContexts[m_CurrentRenderContext];
        glViewport(rc.m_ViewPortX, rc.m_ViewPortY, rc.m_ViewPortWidth, rc.m_ViewPortHeight);
    }
    
    /**
     * Swaps render buffers
     */
    
    inline void swapBuffers(){
        SDL_GL_SwapWindow(m_Window);
    }
    
    /**
     * Adds a shader program to shaders using given source files
     *
     * @param vertexShaderFile File containing vertex shader source
     * @param fragmentShaderFile File containing fragment shader source
     */
    void addShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
    
    
    /**
     * Adds a shader program to shaders using given source files
     *
     * @param vertexShaderFile File containing vertex shader source
     * @param geometryShaderFile File containing geometry shader source
     * @param fragmentShaderFile File containing fragment shader source
     */
    void addShaderProgram(char* vertexShaderFile, char* geometryShaderFile, char* fragmentShaderFile);
    
    /**
     * Use a given shader for rendering
     */
    void useShader() {
        m_CurrentShader = 0;
        glUseProgram(m_Shaders[0]->m_Id);
    }
    
    void useShader(GLuint index){
        m_CurrentShader = index;
        glUseProgram(m_Shaders[index]->m_Id);
    }
    
    GLuint GetShaderId(){
        return m_Shaders[0]->m_Id;
    }
    
    void setVec3(const glm::vec3& vector, const GLchar* uniformName){
        glUniform3f(m_Shaders[m_CurrentShader]->getLocation(uniformName), vector.x, vector.y, vector.z);
    }
    
    void setFloat(GLfloat value, const GLchar* uniformName){
        glUniform1f(m_Shaders[m_CurrentShader]->getLocation(uniformName), value);
    }
    
    /**
     * Sets texture unit to sampler2D for current shader
     **/
    void setUniformSampler2D(GLint textureUnit, const GLchar* sampler2Dname){
        glUniform1i(m_Shaders[m_CurrentShader]->getLocation(sampler2Dname), textureUnit);

    }
    
    /**
     * Sends a given matrix to shader with given uniform name
     *
     * @param matrix Matrix to be sent to shader
     * @param uniformName Name of the uniform in shader
     */
    void setMatrix4fUniform(const glm::mat4& matrix, const GLchar* uniformName){
        glUniformMatrix4fv(m_Shaders[m_CurrentShader]->getLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
    }
    
    /**
     * Specify the value of a uniform variable for the current shader. Binds a texture to a texture unit with given name
     *
     * @param texture Texture id to be bound
     * @param textureUnit Texture unit to bind
     * @param sampler2Dname Name of the sampler uniform in shader
     * @param value What index should be bound
     */
    void bindTexture(GLuint texture, GLenum textureUnit, const GLchar* sampler2Dname, GLint value){
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, texture);
        setUniformSampler2D(value, sampler2Dname);
    }
    
    /**
     * Adds an uniform with given name to current shader
     * 
     * @param uniformName Name of the uniform to be added to the shader
     */
    void addUniform(const GLchar* uniformName){
        m_Shaders[m_CurrentShader]->addUniform(uniformName);
    }
    
    void setCurrentShader(GLint index){
        m_CurrentShader = index;
    }
    
    /**
     * Returns current render context
     *
     * @return render context
     */
    inline RenderContext& getCurrentRenderContext(){
        return m_RenderContexts[m_CurrentRenderContext];
    }
    
    /**
     * Returns render context of certain index 
     * 
     * @param index Index of the render context
     * @return render context
     */
    inline RenderContext& getRenderContextOfIndex(GLuint index){
        return m_RenderContexts[index];
    }
    
    /**
     * Switches current render context
     */
    inline void nextRenderContext(){
        m_CurrentRenderContext = (m_CurrentRenderContext + 1) % m_RenderContexts.size();
    }
    
    /**
     * Returns number of render contexts
     */
    inline GLint getNumberOfRenderContexts() {
        return m_RenderContexts.size();
    }
    
    /**
     * Returns SDL Window that is used for rendering
     */
    inline SDL_Window* getWindow(){
        return m_Window;
    }
    
    inline ShaderProgram* getCurrentShader(){
        return m_Shaders[m_CurrentShader];
    }
    
    
private:
    
    SDL_GLContext m_Context; ///< SDL GL context
    SDL_Window* m_Window; // window for rendering
    
    RenderContext m_RenderContext; ///< render context
    GLuint m_CurrentRenderContext; ///< index of current render context
    std::vector<RenderContext> m_RenderContexts;
    
    ResourceManager m_ResourceManager; ///< resource managing
    
    std::vector<ShaderProgram*> m_Shaders; ///< shaders
    GLuint m_CurrentShader; ///< current shader index

};

}
    
#endif /* GLContext_h */
