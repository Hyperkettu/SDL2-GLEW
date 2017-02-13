//
//  Application.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Application.h"
#include "InputManager.h"
#include "Time.h"

namespace Fox {
    
    GLfloat Time::deltaTime = 0;
    
/**
 * Quits this application
 */
    void Application::quit() {
        m_Quit = true;
    }
    
    void Application::startApplication(){
        
        Uint32 time = 0;
        
        // update until the application is closed
        while (!m_Quit)
        {
            
            Time::deltaTime = 0.001f*(SDL_GetTicks() - time);
            time = SDL_GetTicks();
        
            // handle input
            m_InputManager->handleInput(this);
            
            // draw the entire scene
            drawScene();
            // swap buffers
            m_glContext->swapBuffers();
            
            // wait for next frame
            SDL_Delay(10);
            
        }
        
    }
    
    void Application::drawScene(){
        // clear the screen at first
        clearScreen();
        
        // draw all render contexts
        for(int i = 0; i < m_glContext->getNumberOfRenderContexts(); i++) {
        
            m_glContext->setViewPort();
            
/*            std::vector<glm::vec3> cubePositions;
            
            cubePositions.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));
            cubePositions.push_back(glm::vec3( 2.0f,  5.0f, -15.0f));
            cubePositions.push_back(glm::vec3(-1.5f, -2.2f, -2.5f));
            cubePositions.push_back(glm::vec3(-3.8f, -2.0f, -12.3f));
            cubePositions.push_back(glm::vec3( 2.4f, -0.4f, -3.5f));
            cubePositions.push_back(glm::vec3(-1.7f,  3.0f, -7.5f));
            cubePositions.push_back(glm::vec3( 1.3f, -2.0f, -2.5f));
            cubePositions.push_back(glm::vec3( 1.5f,  2.0f, -2.5f));
            cubePositions.push_back(glm::vec3( 1.5f,  0.2f, -1.5f));
            cubePositions.push_back(glm::vec3(-1.3f,  1.0f, -1.5f));
  */
            std::vector<glm::vec3> spherePositions;
            spherePositions.push_back(glm::vec3( 0.0f,  0.0f,  0.0f));
            spherePositions.push_back(glm::vec3( 20.0f,  10.0f, -15.0f));
            spherePositions.push_back(glm::vec3(-15.5f, -2.2f, -2.5f));
            spherePositions.push_back(glm::vec3(-20.8f, -0.0f, -12.3f));
            spherePositions.push_back(glm::vec3( 17.4f, -0.4f, -30.5f));
            spherePositions.push_back(glm::vec3(-1.7f,  3.0f, -7.5f));
            spherePositions.push_back(glm::vec3( 11.3f, -2.0f, -20.5f));
            spherePositions.push_back(glm::vec3( 15.5f,  6.0f, -2.5f));
            spherePositions.push_back(glm::vec3( 13.5f,  0.2f, -10.5f));
            spherePositions.push_back(glm::vec3(-8.3f,  1.0f, -1.5f));
        
            glEnable(GL_DEPTH_TEST);
            
            // use lighting shader
            m_glContext->useShader(0);
             
            m_glContext->setCameraPosition("viewPos");
            
            // define directional light
            m_glContext->setVec3(glm::vec3(0.4f, 0.4f, 0.4f), "dirLight.ambient");
            m_glContext->setVec3(glm::vec3(0.5f, 0.5f, 0.5f), "dirLight.diffuse");
            m_glContext->setVec3(glm::vec3(1.0f, 1.0f, 1.0f), "dirLight.specular");
           // m_glContext->setVec3(m_glContext->getCurrentRenderContext().m_Camera.m_Front, "dirLight.direction");
            m_glContext->setVec3(glm::vec3(2.0f, -2.0f, -3.0f), "dirLight.direction");
            
            
            // define spot light
            m_glContext->setVec3(glm::vec3(0.2f, 0.2f, 0.2f), "spotLight.ambient");
            m_glContext->setVec3(glm::vec3(0.5f, 0.5f, 0.5f), "spotLight.diffuse");
            m_glContext->setVec3(glm::vec3(1.0f, 1.0f, 1.0f), "spotLight.specular");
            
            m_glContext->setFloat(1.0f, "spotLight.constant");
            m_glContext->setFloat(0.09f, "spotLight.linear");
            m_glContext->setFloat(0.032f, "spotLight.quadratic");
            
            m_glContext->setFloat(glm::cos(glm::radians(12.5f)), "spotLight.cutOff");
            m_glContext->setFloat(glm::cos(glm::radians(17.5f)), "spotLight.outerCutOff");
            
            m_glContext->setVec3(m_glContext->getCurrentRenderContext().m_Camera.m_Position, "spotLight.position");
            m_glContext->setVec3(m_glContext->getCurrentRenderContext().m_Camera.m_Front, "spotLight.direction");
            
            m_glContext->setViewUniform("view");
            m_glContext->setProjectionUniform("projection");
            
            
          //  m_Cube.draw(m_glContext, 10, cubePositions);
            
            m_Sphere.draw(m_glContext, 10, spherePositions);
            
            glm::mat4 model;
            
            model = glm::translate(model, m_p);
           // model = glm::rotate(model, (GLfloat)SDL_GetTicks()* 0.00001f * 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
           // model = glm::rotate(model, (GLfloat)SDL_GetTicks()* 0.00001f * 50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
            m_glContext->setMatrix4fUniform(model, "model");
        
            //m_Cube.draw(m_glContext);
           // m_Cylinder.draw(m_glContext);
            
            m_glContext->getCurrentRenderContext().updateFrustum(model);
            //bool in = m_glContext->getCurrentRenderContext().m_Frustum.sphereIsInsideFrustum(glm::vec3(0,0,0), 1.0f);
            //m_Sphere.draw(m_glContext);
            
            
            model = glm::mat4();
            m_glContext->setMatrix4fUniform(model, "model");
            m_Plane.draw(m_glContext);
          //  m_Plane.drawWireframe(m_glContext);
      
        
        // use lamp shader
            m_glContext->useShader(1);
            
            model = glm::mat4();
            model = glm::translate(model, m_lightPos);
            model = glm::scale(model, glm::vec3(0.2f));
            
            m_glContext->setMatrix4fUniform(model, "model");
            m_glContext->setViewUniform("view");
            m_glContext->setProjectionUniform("projection");
        
            m_CubeLamp.draw(m_glContext);

            m_glContext->nextRenderContext();
        }
        
        
        
    }
    
    void Application::clearScreen(){
    
        glClearColor(0.3f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
