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
            
            // draw the scene to depth buffer
         /*   m_ShadowMap->renderToDepthBuffer(m_glContext);
            renderSceneToDepthBuffer();
            // switch back to back buffer
            m_ShadowMap->switchToBackBuffer();
            
            // Reset viewport
            glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_ShadowMap->visualizeDepthBuffer(m_glContext);*/
            
            renderScene();

            m_glContext->nextRenderContext();
        }
        
        
        
    }
    
    void Application::renderSceneToDepthBuffer(){
        
        glEnable(GL_DEPTH_TEST);

        
     /*   std::vector<glm::vec3> positions;
        positions.push_back(glm::vec3(0.0f, 1.5f, 0.0));
        positions.push_back(glm::vec3(2.0f, 0.0f, 1.0));
        positions.push_back(glm::vec3(-1.0f, 0.0f, 2.0));*/
        
        // DRAW TREES
        m_Cylinder.drawToDepthBuffer(m_glContext, m_CylinderPositions);
        m_Sphere.drawToDepthBuffer(m_glContext, m_SpherePositions);
        
        glm::mat4 model;
        model = glm::mat4();
        m_glContext->setMatrix4fUniform(model, "model");
        // DRAW GROUND
        m_Plane.drawToDepthBuffer(m_glContext);
        
        // Cubes
    /*    model = glm::mat4();
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        m_glContext->setMatrix4fUniform(model, "model");
        m_Cube.drawToDepthBuffer(m_glContext);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        m_glContext->setMatrix4fUniform(model, "model");
        m_Cube.drawToDepthBuffer(m_glContext);
        model = glm::mat4();
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
        model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
       // model = glm::scale(model, glm::vec3(0.5));
        m_glContext->setMatrix4fUniform(model, "model");
        m_Cube.drawToDepthBuffer(m_glContext);
      */
        
        // DRAW TREES
        //m_Cylinder.drawToDepthBuffer(m_glContext, m_CylinderPositions);
        //m_Sphere.drawToDepthBuffer(m_glContext, m_SpherePositions);
        
        /*
        glm::mat4 model;
        model = glm::mat4();
        m_glContext->setMatrix4fUniform(model, "model");
        // DRAW GROUND
        m_Plane.drawToDepthBuffer(m_glContext);*/
        
      //  model = glm::mat4();
      //  model = glm::rotate(model, (GLfloat)SDL_GetTicks()* 0.00001f * 50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
      //  model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
      //  m_glContext->setMatrix4fUniform(model, "model");
     //   m_Nano.draw(m_glContext);
    
    }
    
    void Application::renderScene(){
    
        m_glContext->setViewPort();
        
        glEnable(GL_DEPTH_TEST);
        
        // use lighting shader (diffuse specular)
        m_glContext->useShader(3);
        
        m_glContext->setCameraPosition("viewPos");
        
        // define directional light
        m_glContext->setVec3(glm::vec3(0.0f, 0.0f, 0.05f), "dirLight.ambient");
        m_glContext->setVec3(glm::vec3(1.0f, 0.95f, 0.65f), "dirLight.diffuse");
        m_glContext->setVec3(glm::vec3(1.0f, 1.0f, 1.0f), "dirLight.specular");
        //  m_glContext->setVec3(m_glContext->getCurrentRenderContext().m_Camera.m_Front, "dirLight.direction");
        m_glContext->setVec3(glm::vec3(2.0f, -3.0f, 3.0f), "dirLight.direction");
        //  m_glContext->setVec3(glm::vec3(1.0f, -1.0f, -10.0f), "dirLight.direction");
        
        
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
        
        // DRAW TREES
        m_Sphere.draw(m_glContext, m_SpherePositions.size(), m_SpherePositions);
        m_Cylinder.draw(m_glContext, m_CylinderPositions.size(), m_CylinderPositions);
        
        glm::mat4 model;
        
        model = glm::mat4();
        m_glContext->setMatrix4fUniform(model, "model");
        
        // DRAW GROUND
        m_Plane.draw(m_glContext);
        //  m_Plane.drawWireframe(m_glContext);
        
        // use lighting shader (bumped diffuse specular)
        m_glContext->useShader(0);
        
        m_glContext->setCameraPosition("viewPos");
        
        // define directional light
        m_glContext->setVec3(glm::vec3(0.0f, 0.0f, 0.05f), "dirLight.ambient");
        m_glContext->setVec3(glm::vec3(1.0f, 0.95f, 0.65f), "dirLight.diffuse");
        m_glContext->setVec3(glm::vec3(1.0f, 1.0f, 1.0f), "dirLight.specular");
        m_glContext->setVec3(glm::vec3(2.0f, -3.0f, 3.0f), "dirLight.direction");
        
        
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
        
        model = glm::mat4();
        model = glm::rotate(model, (GLfloat)SDL_GetTicks()* 0.00001f * 50.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        m_glContext->setMatrix4fUniform(model, "model");
        m_Nano.draw(m_glContext);
        //  m_Nano.drawWireframe(m_glContext);
        
        /*   // use lamp shader
         m_glContext->useShader(1);
         
         model = glm::mat4();
         model = glm::translate(model, m_lightPos);
         model = glm::scale(model, glm::vec3(0.2f));
         
         m_glContext->setMatrix4fUniform(model, "model");
         m_glContext->setViewUniform("view");
         m_glContext->setProjectionUniform("projection");
         
         m_CubeLamp.draw(m_glContext);
         */
        
        m_Skybox.draw(m_glContext);
    
    
    }
    
    void Application::clearScreen(){
    
        glClearColor(0.3f, 0.7f, 1.0f, 1.0f);
       // glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}
