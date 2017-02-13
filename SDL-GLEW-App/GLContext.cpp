//
//  GLContext.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/27/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "GLContext.h"

namespace Fox {

    void GLContext::addShaderProgram(char* vertexShaderFile, char* fragmentShaderFile) {
    
        GLint vertexShaderFileSize;
        char* vertexShaderData = ResourceManager::loadFile(vertexShaderFile, vertexShaderFileSize);
        
        GLint fragmentShaderFileSize;
        char* fragmentShaderData = ResourceManager::loadFile(fragmentShaderFile, fragmentShaderFileSize);
        
        m_Shaders.push_back(new ShaderProgram(vertexShaderData, vertexShaderFileSize, fragmentShaderData, fragmentShaderFileSize));
    }

}
