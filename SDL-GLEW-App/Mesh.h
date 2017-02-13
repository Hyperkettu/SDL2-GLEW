//
//  Mesh.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/4/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Mesh_h
#define Mesh_h

#include <GL/glew.h>

#include <vector>

#include "Vertex.h"
#include "cubeData.h"
#include "GLContext.h"
#include "Texture.h"
#include "TextureManager.h"
#include "BoundingVolume.h"

namespace Fox {
    
    static std::vector<const GLchar*> textureToUniformName = {"material.diffuse", "material.specular", nullptr, nullptr, nullptr, nullptr};
    
    class MeshBase {
    public:
        
        MeshBase(){
            
            m_Material.m_Shininess = 32.0f;
            m_Material.m_Textures = std::vector<Texture*>(Texture::TextureType_Max);
        }
        
        
        /**
         * Adds a texture of certain type to this mesh's material
         *
         * @param texture Texture to be added
         */
        void addTexture(Texture* texture){
            GLuint index = (GLuint) texture->m_Type;
            m_Material.m_Textures[index] = texture;
            
        }
        
        /**
         * Computes the bounding sphere of this mesh
         *
         * @param vertices Vertices of the mesh
         */
        void computeBoundingSphere(std::vector<Vertex>& vertices){
            m_BoundingSphere = BoundingSphere(vertices);
        }
    
        // material class
        class Material {
        public:
        
            Material(){}
        
            GLfloat m_Shininess;
            std::vector<Texture*> m_Textures; /// all texture types
        };
    
        Material m_Material; ///< material of the mesh
        BoundingSphere m_BoundingSphere; ///< bounding sphere
    };
        
template <class V = Vertex> class FMesh : public MeshBase {
    
    public:
    
    FMesh(){}
    
    FMesh(std::vector<V>& vertices, GLenum usage);
    
    void draw(GLContext* gl){
        
        // bind all existing textures
        for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
        
            Texture* texture = m_Material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                
                gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
        
        }
        
        glActiveTexture(GL_TEXTURE0);
        
        if(gl->getCurrentShader()->hasUniform("material.shininess"))
            gl->setFloat(m_Material.m_Shininess, "material.shininess");
        
        // render this mesh
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        glBindVertexArray(0);
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
     
    }
    
    void draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions){
    
        // bind all existing textures
        for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
            
            Texture* texture = m_Material.m_Textures[i];
            
            // if there is a texture
            if(texture != nullptr){
                gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
            }
            
        }
        
        glActiveTexture(GL_TEXTURE0);
        
        glBindVertexArray(m_Vao);
        
        gl->setFloat(m_Material.m_Shininess, "material.shininess");
        
        // render n times
        for(GLuint i = 0; i < n; i++)
        {
            glm::mat4 model = glm::mat4();
            model = glm::translate(model, positions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            gl->setMatrix4fUniform(model, "model");
            
            glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        }
        
        glBindVertexArray(0);
        
        // reset bound textures
        for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
    
    void drawWireframe(GLContext* gl){
        // enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        glBindVertexArray(m_Vao);
        glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
        glBindVertexArray(0);
        
        // disable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    }
    
    std::vector<V> m_Vertices; ///< vertices
    std::vector<GLuint> m_Indices; ///< index data
    
private:
    
    GLuint m_Vao; ///< vertex array id
    GLuint m_Vbo; ///< vertex buffer object
    
    };
    
    template <class V = Vertex> class Mesh : public MeshBase {
    
    public:
        
        Mesh(){}
        
        Mesh(std::vector<V>& vertices, std::vector<GLuint>& indices, GLenum usage);
        
        void draw(GLContext* gl){
            
            // bind all existing textures
            for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
                
                Texture* texture = m_Material.m_Textures[i];
                
                // if there is a texture
                if(texture != nullptr){
                    gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
                }
                
            }
            
            gl->setFloat(m_Material.m_Shininess, "material.shininess");
            
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0); //BUFFER_OFFSET(0));
            glBindVertexArray(0);
            
            // reset bound textures
            for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        
        void drawWireframe(GLContext* gl){
            // enable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            
            // disable wireframe mode
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            
        }
        
        void draw(GLContext* gl, GLuint n, std::vector<glm::vec3>& positions){
            
            // bind all existing textures
            for(GLuint i = 0; i < m_Material.m_Textures.size(); i++){
                
                Texture* texture = m_Material.m_Textures[i];
                
                // if there is a texture
                if(texture != nullptr){
                    gl->bindTexture(texture->m_Id, GL_TEXTURE0 + i, textureToUniformName[(GLuint) texture->m_Type], i);
                }
                
            }
            
            glActiveTexture(GL_TEXTURE0);
            
            glBindVertexArray(m_Vao);
            
            if(gl->getCurrentShader()->hasUniform("material.shininess"))
                gl->setFloat(m_Material.m_Shininess, "material.shininess");
            
            
            
            // render n times
            for(GLuint i = 0; i < n; i++)
            {
                glm::mat4 model = glm::mat4();
                model = glm::translate(model, positions[i]);
                gl->setMatrix4fUniform(model, "model");
                
                gl->getCurrentRenderContext().updateFrustum(model);
             
                // use frustum culling and bounding volume to determine if sphere is inside
                if(gl->getCurrentRenderContext().m_Frustum.sphereIsInsideFrustum(m_BoundingSphere.m_Center, m_BoundingSphere.m_Radius)){
                    glDrawElements(GL_TRIANGLES, (GLsizei) m_Indices.size(), GL_UNSIGNED_INT, 0);
                
                }
            }
            
            glBindVertexArray(0);
            
            // reset bound textures
            for (GLuint i = 0; i < m_Material.m_Textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
        }
        
        /**
         * Calculates normals for vertices based on indices
         *
         * @param vertices Vertex array
         * @param indices Index array
         */
        static void calculateNormals(std::vector<Vertex>& vertices, const std::vector<GLuint>& indices){
            
            GLfloat* normal = new GLfloat[3 * vertices.size()];
            
            // initialize normal array with zeros
            for(GLuint i = 0; i < 3 * vertices.size(); i++){
                normal[i] = 0;
            }
            
            // calculate normals with cross product
            for (GLuint e = 0; e < indices.size(); e+=3) {
                
                Vertex& v = vertices[indices[e]];
                Vertex& v2 = vertices[indices[e + 1]];
                Vertex& v3 = vertices[indices[e + 2]];
                
                // triangle end points
                glm::vec3 a = v.m_Position;
                glm::vec3 b = v2.m_Position;
                glm::vec3 c = v3.m_Position;
                // cross product
                glm::vec3 n = glm::cross(c - b, a - b);
                
                // add normals to every participating vertex
                normal[3*indices[e]] += n.x;
                normal[3*indices[e] + 1] += n.y;
                normal[3*indices[e] + 2] += n.z;
                
                normal[3*indices[e+1]] += n.x;
                normal[3*indices[e+1] + 1] += n.y;
                normal[3*indices[e+1] + 2] += n.z;
                
                normal[3*indices[e+2]] += n.x;
                normal[3*indices[e+2] + 1] += n.y;
                normal[3*indices[e+2] + 2] += n.z;
                
            }
            
            // normalize normals
            for(GLuint i = 0; i < 3 * vertices.size(); i+=3){
                
                glm::vec3 n = glm::vec3(normal[i], normal[i+1], normal[i+2]);
                n = glm::normalize(n);
                
                normal[i] = n.x;
                normal[i+1] = n.y;
                normal[i+2] = n.z;
                
            }
            // set normals to vertices
            for(GLuint i = 0; i < vertices.size(); i++){
                
                Vertex& vertex = vertices[i];
                vertex.m_Normal = glm::vec3(normal[3*i], normal[3*i+1], normal[3*i+2]);
            }
            
            // release array memory
            delete[] normal;
            
        }
        
        std::vector<V> m_Vertices; ///< vertices
        std::vector<GLuint> m_Indices; ///< index data
    
    private:
        GLuint m_Vao; ///< vertex array id
        GLuint m_Vbo; ///< vertex buffer object
        GLuint m_Ibo; ///< index buffer object
        
    };

    /**
     * Creates a cube 
     *
     * @return cube mesh
     **/
    template <class V>
    static FMesh<V> createCube() {
        
        FMesh<V> mesh;
        
        std::vector<V> data = convertCubeData<V>();
        mesh = FMesh<V>(data, GL_STATIC_DRAW);
        
        return mesh;
    }
    
    static MeshBase* import(const GLchar* fileName){
        return new MeshBase;
    }
    
    /**
     * Generates plane vertex data
     *
     * @param vertex data
     */
    template<class V> static std::vector<V> getGroundData(const GLchar* filePath, GLint* width, GLint* height){
        return std::vector<V>();
    }
    
    /**
     * Generates plane index data
     *
     * @return index data
     */
    template<class V> static std::vector<V> getPlaneIndices(GLint w, GLint h){
        return std::vector<V>();
    }
    
    /**
     * Generates ground vertex data for type Vertex based on height map
     *
     * @return vertex data
     */
    template<> static
    std::vector<Vertex> getGroundData(const GLchar* filePath, GLint* width, GLint* height) {
        
        std::vector<Vertex> data;
        
        GLfloat dimension = 1.0f;
        
       SDL_Surface* image = Texture::readTexture(filePath);
        
        Uint8 r,g,b;
        
        Uint32* p = (Uint32*) image->pixels;
        
        GLint w = image->w;
        GLint h = image->h;
        *width = w;
        *height = h;
        
        for(int i = -w/2; i <= w/2; i++){
            
            for(int j = - h/2; j <= h/2; j++){
                
                GLint x = i+w/2;
                GLint y = j+h/2;
                
                if(x == w){
                    x--;
                }
                
                if(y == h){
                    y--;
                }
                
                // sample height map
                SDL_GetRGB(p[y*image->w + x], image->format, &r, &g, &b);
                
                GLint heightMap = (GLint)(0.21f * r + 0.72f * g + 0.07f * b);
                
                //GLint heightMap = (GLint) r;
                GLfloat h = (GLfloat)(heightMap/255.0f) * 10.0f - 10.0f;
                Vertex v;
                v.m_Position = glm::vec3(i*dimension, h, j*dimension);
                
                
                v.m_Normal = glm::vec3(0.0f, 1.0f, 0.0f);
                v.m_TexCoords = glm::vec2((GLfloat) (i + 5), (GLfloat)(j + 5));
                data.push_back(v);
            }
        }
        
        SDL_FreeSurface(image);
        
        return data;
    }
    
    /**
     * Generates plane index data
     *
     * @return index data
     */
    template<> static
    std::vector<GLuint> getPlaneIndices(GLint w, GLint h){
    
        std::vector<GLuint> indices;
        
        for(int i = 0; i < w; i++){
            for(int j = 0; j < h; j++){
                // first triangle
                indices.push_back((h+1)*i + j);
                indices.push_back((h+1) * (i+1) + j + 1);   // swap
                indices.push_back((h+1) * (i+1) + j);      // swap
                // second triangle
                indices.push_back((h+1)*i + j);
                indices.push_back((h+1)*i + j + 1); // swap2
                indices.push_back((h+1) * (i+1) + j + 1); // swap2
            
            }
            
         /*   for(int i = 0; i < indices.size(); i++){
                std::cout << indices[i] << std::endl;
            }*/
        }
        return indices;
    }
    
    template<class V> static
    std::vector<V> getCylinderData(GLuint number, GLfloat height, GLfloat radius) {
        return std::vector<V>();
    }
    
    template<class V> static
    std::vector<V> getCylinderIndices(GLuint number) {
        return std::vector<V>();
    }
    
    template<> static
    std::vector<Vertex> getCylinderData(GLuint number, GLfloat height, GLfloat radius) {
        
        std::vector<Vertex> vertices;
        
        GLfloat deltaAngle =  2 * glm::pi<GLfloat>()/(GLfloat)(number);
        
        for(GLuint face = 0; face <= number; face++){
            
            Vertex vertex, vertex2;
            
            GLfloat x = radius * cos(face*deltaAngle);
            GLfloat z = radius * sin(face*deltaAngle);
            
            glm::vec3 normal = glm::normalize(glm::vec3(x, 0, z));
            
            glm::vec3 pos = glm::vec3(x, -height/2.0f, z);
            glm::vec3 pos2 = glm::vec3(x, height/2.0f, z);
            
            if(x == 0 && z == 0){
                std::cout << face << std::endl;
            }
            
            vertex.m_Position = pos;
            vertex.m_Normal = normal;
            vertex.m_TexCoords = glm::vec2(1/(GLfloat)(number)*face, 0.0f);
            
            vertices.push_back(vertex);
            
            vertex2.m_Position = pos2;
            vertex2.m_Normal = normal;
            vertex2.m_TexCoords = glm::vec2(1/(GLfloat)(number)*face, 1.0f);
            vertices.push_back(vertex2);
        }
        
        
        return vertices;
    }
    
    template<> static
    std::vector<GLuint> getCylinderIndices(GLuint number) {
        
        std::vector<GLuint> indices;
        
        for(int i = 0; i <= number; i++){
            // first triangle
            indices.push_back(2 * i);
            indices.push_back(2 * (i + 1)); 
            indices.push_back(2 * (i + 1) + 1);
            // second triangle
            indices.push_back(2 * i);
            indices.push_back((2 * (i + 1)) + 1);
            indices.push_back(2 * i + 1);
        }
        
        return indices;
    }
    
    template<class V>
    static std::vector<V> sphere(int sides, GLfloat r){
        return std::vector<V>();
    }
    
    /**
     * Calculates vertices of a sphere with sides^2 faces and r radius
     */
    template<>
    static std::vector<Vertex> sphere(int sides, GLfloat r){
        
        std::vector<Vertex> vertices;
        
        GLfloat angle = 0;
        GLfloat PI = glm::pi<GLfloat>();
        GLfloat dAngle = 2*PI/static_cast<GLfloat>(sides);
        GLfloat dh = 2*r/static_cast<GLfloat>(sides);
        
        GLfloat x = 0, y = 0, z = 0;
        
        // circle around y axis
        for(int i = 0; i <= sides; i++){
            
            GLuint k = 0;
            
            for(GLfloat h = 0; h <= 2*r; h += dh){
                
                y = h - r;
                
                // use sphere function
                x = cos(angle)*(sqrt(r*r - (h - r)*(h - r)));
                z = -sin(angle)*(sqrt(r*r - (h - r)*(h - r)));
                
                Vertex v;
                v.m_Position = glm::vec3(x,y,z);
                v.m_TexCoords = glm::vec2(1/(GLfloat)(sides)*i, 1/(GLfloat)(sides)*k);
                v.m_Normal = glm::normalize(glm::vec3(x,y,z));
                vertices.push_back(v);
                k++;
            }
            angle += dAngle;
        }
        
        return vertices;
    }
    
    /**
     * Creates a ground based on height map
     *
     * @return created plane
     */
    template <class V>
    static Mesh<V> createGround(const GLchar* filePath){

        GLint w, h;
        std::vector<V> vertices = getGroundData<V>(filePath, &w, &h);
        std::vector<GLuint> indices = getPlaneIndices<GLuint>(w, h);
        
        Mesh<V>::calculateNormals(vertices, indices);
        
        Mesh<V> mesh = Mesh<V>(vertices, indices, GL_STATIC_DRAW);
        return mesh;
        
    }
    
    /**
     * Creates a cylinder
     *
     * @param number Number of faces
     * @param height Height of the cylinder
     * @param radius Radius of the cylinder
     */
    template <class V>
    static Mesh<V> createCylinder(GLuint number, GLfloat height, GLfloat radius){
    
        std::vector<V> vertices = getCylinderData<V>(number, height, radius);
        std::vector<GLuint> indices = getCylinderIndices<GLuint>(number);
        Mesh<V> mesh = Mesh<V>(vertices, indices, GL_STATIC_DRAW);
        return mesh;
    }
    
    /**
     * Creates a sphere
     *
     * @param sides Number of faces per dimension
     * @param radius Radius of the sphere
     * @return sphere mesh
     */
    
    template <class V>
    static Mesh<V> createSphere(GLuint sides, GLfloat radius){
    
        std::vector<V> vertices = sphere<V>(sides, radius);
        std::vector<GLuint> indices = getPlaneIndices<GLuint>(sides, sides);
     //   Mesh<V>::calculateNormals(vertices, indices);
        return Mesh<V>(vertices, indices, GL_STATIC_DRAW);
    }
}

#endif /* Mesh_h */
