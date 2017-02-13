//
//  Application.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Application_h
#define Application_h

#include <iostream>

#include <SDL2/SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "GLContext.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Time.h"
#include "Camera.h"
#include "Vector.h"
#include "Matrix.hpp"
#include "Ray.hpp"
#include "Mesh.h"
#include "cubeData.h"
#include "Time.h"

#include "InputManager.h"
#include "TextureManager.h"
#include "Model.h"

namespace Fox {
    
class InputManager;
    
    
/**
 * Application for OpenGL rendering
 */
class Application {

public:
    
    /**
     * Creates an OpenGL application with window width and height
     *
     * @param appName Name of the application
     * @param width Window width
     * @param height Window height
     * @param fullscreen Allow full screen mode
     */
    Application(const std::string& appName, GLint width, GLint height, bool fullscreen = false) {
        
        // initialize SDL
        SDL_Init(SDL_INIT_EVERYTHING);
        
        // use OpenGL version 4.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        
        // create window
        SDL_Window* window;
        
        if(fullscreen){
            window = SDL_CreateWindow(appName.c_str(), 100, 100, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
        } else {
            window = SDL_CreateWindow(appName.c_str(), 100, 100, width, height, SDL_WINDOW_OPENGL);
        }
        m_IsFullScreen = fullscreen;
        
        // create OpenGL context for the window
        m_glContext = new GLContext(window, m_ResourceManager);
        
        m_ScreenWidth = width;
        m_ScreenHeight = height;
        
        // load support for the JPG and PNG image formats
        int flags = IMG_INIT_JPG | IMG_INIT_PNG;
        int initted = IMG_Init(flags);
        
        // check if SDL2_Image initialization succeeded
        if( (initted & flags) != flags) {
            std::cout << "IMG_Init: Failed to init required jpg and png support!" << std::endl;
            std::cout << "IMG_Init: " << IMG_GetError() << std::endl;
            
        }
        // init input manager
        m_InputManager = new InputManager;
        
        // add render contexts
        m_glContext->addRenderContext(0, 0, (GLfloat)m_ScreenWidth, (GLfloat) m_ScreenHeight, 45.0f, 0.1f, 300.0f);
    //    m_glContext->addRenderContext((GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenHeight/(GLfloat)2, 45.0f, 0.1f, 100.0f);
        
     //   m_glContext->addRenderContext(0, (GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenHeight/(GLfloat)2, 45.0f, 0.1f, 100.0f);
        
     //   m_glContext->addRenderContext((GLfloat)m_ScreenWidth/(GLfloat)2, 0, (GLfloat)m_ScreenWidth/(GLfloat)2, (GLfloat)m_ScreenHeight, 45.0f, 0.1f, 100.0f);
        
        // starts the program
        start();
        
    }
    
    /**
     * Destroy application
     */
    ~Application(){
        
        // delete texture manager
        delete TextureManager::Instance();
        // delete gl context
        delete m_glContext;
        
        // quit image library
        IMG_Quit();
        // quit SDL
        SDL_Quit();
    }
    
    /**
     * Application update loop
     */
    
    void startApplication();
    
    /**
     * Starts this application
     */
    void start(){
        // init application and start update loop
        initializeData();
        startApplication();
    }
    
    /**
     * Quits this application
     */
    void quit();
    
    /**
     * Draws the entire scene
     */
    void drawScene();
    
    /**
     * Clears the screen
     */
    void clearScreen();
    
    /**
     */
    void initializeData(){
        
        TextureManager* textureManager = TextureManager::Instance();
        
        textureManager->loadTexture("Textures/container.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/grassplain.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/brown.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/tree.png", Texture::Diffuse);
        textureManager->loadTexture("Textures/container_specular.png", Texture::Specular);
        textureManager->loadTexture("Textures/white.png", Texture::Specular);
        textureManager->loadTexture("Textures/black.png", Texture::Specular);
        
        m_Cube = createCube<Vertex>();
        
        
        m_Cube.addTexture(textureManager->getTexture("Textures/container.png"));
        m_Cube.addTexture(textureManager->getTexture("Textures/container_specular.png"));
        
        m_Sphere = createSphere<Vertex>(32, 1.0f);
        m_Sphere.addTexture(textureManager->getTexture("Textures/grassplain.png"));
        m_Sphere.addTexture(textureManager->getTexture("Textures/black.png"));
        
        m_Sphere.computeBoundingSphere(m_Sphere.m_Vertices);
        
        m_CubeLamp = createCube<VertexP>();
        
        m_Cylinder = createCylinder<Vertex>(32, 4.0f, 1.15f);
        m_Cylinder.addTexture(textureManager->getTexture("Textures/tree.png"));
        
        m_Plane = createGround<Vertex>("Textures/height.png");
        m_Plane.addTexture(textureManager->getTexture("Textures/grassplain.png"));
    
        // define spotlight shader
     //   m_glContext->addShaderProgram("Shaders/phong-diffuse-specular.vert", "Shaders/spotLight.frag");
     //   m_glContext->addShaderProgram("Shaders/phong-diffuse-specular.vert", "Shaders/directional.frag");
       
        m_glContext->addShaderProgram("Shaders/phong-diffuse-specular.vert", "Shaders/scenelighting.frag");
        
        m_glContext->addUniform("viewPos");
        
        // setup spot light
        m_glContext->addUniform("spotLight.position");
        m_glContext->addUniform("spotLight.direction");
        m_glContext->addUniform("spotLight.ambient");
        m_glContext->addUniform("spotLight.diffuse");
        m_glContext->addUniform("spotLight.specular");
        m_glContext->addUniform("spotLight.constant");
        m_glContext->addUniform("spotLight.linear");
        m_glContext->addUniform("spotLight.quadratic");
        m_glContext->addUniform("spotLight.cutOff");
        m_glContext->addUniform("spotLight.outerCutOff");
        
        // setup directional light
        m_glContext->addUniform("dirLight.direction");
        m_glContext->addUniform("dirLight.ambient");
        m_glContext->addUniform("dirLight.diffuse");
        m_glContext->addUniform("dirLight.specular");
        
        
        // setup material
        m_glContext->addUniform("material.diffuse");
        m_glContext->addUniform("material.specular");
        m_glContext->addUniform("material.shininess");
        
        
        m_glContext->addUniform("model");
        m_glContext->addUniform("view");
        m_glContext->addUniform("projection");
        
        // define lamp shader
        m_glContext->addShaderProgram("Shaders/transform3d-simplified.vert", "Shaders/white.frag");
        m_glContext->setCurrentShader(1);
        m_glContext->addUniform("model");
        m_glContext->addUniform("view");
        m_glContext->addUniform("projection");
        
        
      //  m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        m_lightPos = glm::vec3(0.0f, 0.0f, 5.0f);
     
    }
    
    void SetContainerPositionToMousePosition(){
        
        //m_p = m_glContext->getCurrentRenderContext().castRay(m_InputManager->m_MouseX, m_ScreenHeight - m_InputManager->m_MouseY);
        m_p = m_glContext->getRenderContextOfIndex(0).castRay(m_InputManager->m_MouseX, m_ScreenHeight - m_InputManager->m_MouseY);
    }
    
    void moveMouseForward(){
        m_glContext->getCurrentRenderContext().m_Camera.moveForward(Time::deltaTime);
    }
    
    void moveMouseBackward(){
        m_glContext->getCurrentRenderContext().m_Camera.moveBackward(Time::deltaTime);
    }
    void moveCameraRight(){
        m_glContext->getCurrentRenderContext().m_Camera.moveRight(Time::deltaTime);
    }
    
    void moveCameraLeft(){
        m_glContext->getCurrentRenderContext().m_Camera.moveLeft(Time::deltaTime);
    }
    
    void moveCameraForwardZ(){
        m_glContext->getCurrentRenderContext().m_Camera.moveForwardZ(Time::deltaTime);
    }
    
    void moveCameraBackwardZ(){
        m_glContext->getCurrentRenderContext().m_Camera.moveBackwardZ(Time::deltaTime);
    }
    
    
    /**
     * Toggles full screen on and off
     */
    void toggleFullScreen(){
        
        // if is full screen, switch to windowed mode
        if(m_IsFullScreen){
            SDL_SetWindowFullscreen(m_glContext->getWindow(), 0);
        } else { // not in fullscreen
            SDL_SetWindowFullscreen(m_glContext->getWindow(), SDL_WINDOW_FULLSCREEN);
        }
        
        m_IsFullScreen = !m_IsFullScreen;
    }
    
private:
    
    FMesh<Vertex> m_Cube;
    FMesh<VertexP> m_CubeLamp;
    Mesh<Vertex> m_Plane;
    Mesh<Vertex> m_Cylinder;
    Mesh<Vertex> m_Sphere;
    
    glm::vec3 m_p;
    glm::vec3 m_lightPos;
    
    bool m_IsFullScreen; ///< tells if this application is in full screen mode
    
    GLint m_ScreenWidth, m_ScreenHeight; // dimensions of the screen
    
    GLContext* m_glContext; ///< OpenGL context
    bool m_Quit; ///< should the program quit
    InputManager* m_InputManager; ///< manager for input handling
    ResourceManager m_ResourceManager; ///< manager for resource handling
};

}
    
#endif /* Application_h */