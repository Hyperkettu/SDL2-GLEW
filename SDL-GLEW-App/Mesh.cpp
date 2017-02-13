//
//  Mesh.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <algorithm>

#include "glm/glm.hpp"

#include "Mesh.h"

namespace Fox {

    template<>
    FMesh<Vertex>::FMesh(std::vector<Vertex>& vertices, GLenum usage) {
     
        // copy data to vector
        m_Vertices.resize(vertices.size());
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        std::cout << "Vertex" << std::endl;
        
        // create vertex buffer object
        glGenBuffers(1, &m_Vbo);
        
        // create vertex array
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        // fill buffer with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), (const GLvoid*) vertices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex),(GLvoid*) 0);// (GLvoid*)&Vertex::m_Position);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (3 * sizeof(GLfloat)));
        // &Vertex::m_Normal;
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (6 * sizeof(GLfloat)));
        // &Vertex::m_TexCoords;
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
    
    template<>
    FMesh<VertexP>::FMesh(std::vector<VertexP>& vertices, GLenum usage) {
        
        // copy data to vector
        m_Vertices.resize(vertices.size());
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        std::cout << "VertexP" << std::endl;
        
        // create vertex buffer object
        glGenBuffers(1, &m_Vbo);
        
        // create vertex array
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        // fill buffer with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexP) * vertices.size(), (const GLvoid*) vertices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexP),(GLvoid*) 0);// (GLvoid*)&Vertex::m_Position);
        glEnableVertexAttribArray(0);
        
        glBindVertexArray(0);
    }
    
    template<>
    Mesh<Vertex>::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, GLenum usage){
        
        m_Vertices.resize(vertices.size());
        m_Indices.resize(indices.size());
        
        std::copy(vertices.begin(), vertices.end(), m_Vertices.begin());
        std::copy(indices.begin(), indices.end(), m_Indices.begin());
        
        // generate vertex and index buffer objects
        glGenBuffers(1, &m_Vbo);
        glGenBuffers(1, &m_Ibo);
        
        // create vertex array object
        glGenVertexArrays(1, &m_Vao);
        
        glBindVertexArray(m_Vao);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_Vertices.size(), (const GLvoid*) m_Vertices.data(), usage);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), (const GLvoid*) m_Indices.data(), usage);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (3 * sizeof(GLfloat)));
        glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (GLvoid*) (6 * sizeof(GLfloat)));
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0);
    }
    
    
    
}
