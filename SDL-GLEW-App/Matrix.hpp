//
//  Matrix.hpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/29/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Matrix_hpp
#define Matrix_hpp

#include <iostream>

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fox {
    
    /*
     * Generates a view port matrix
     * @param x lower left corner x
     * @param y lower left corner y
     * @param width of the view port
     * @param height of the view port
     */
    glm::mat4 viewport (GLfloat x, GLfloat y, GLfloat width, GLfloat height);
    
    /**
     * Transforms a 3x3 matrix to 4x4 matrix
     * @ param matrix Martix to be transformed
     */
    glm::mat4 toMat4(const glm::mat3& matrix);
    
    /**
     * Transforms a 4x4 matrix to 3x3 matrix
     * @param matrix Matrix to be transformed
     */
    glm::mat3 toMat3(const glm::mat4& matrix);
    
    /**
     * Prints given matrix
     * @param m matrix to be printed
     */
    void printMatrix(const glm::mat4& m);
    
}

#endif /* Matrix_hpp */
