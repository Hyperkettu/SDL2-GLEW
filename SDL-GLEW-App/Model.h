//
//  Model.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/13/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Model_h
#define Model_h

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "GLContext.h"
#include "Mesh.h"

namespace Fox {
    
    /**
     * Model consists of several meshes
     */
    class Model {
    
    public:
        Model(){}
        
        
        /**
         * Creates a model from data path
         *
         * @param path Path to the model data
         */
        Model(GLchar* path){
            loadModel(path);
        }
        
        /**
         * Draws this model 
         *
         * @param gl GLContext
         */
        void draw(GLContext* gl) {
            // draw all meshes
            for(GLuint i = 0; i < m_Meshes.size(); i++){
                m_Meshes[i].draw(gl);
            }
        }
        
    private:
        
        /**
         * Loads the model from path
         *
         * @param path Path of the model
         */
        void loadModel(std::string path);
        
        /**
         * Processes an aiNode
         *
         * @param node aiNode to be processed
         * @param scene aiScene
         */
        void processNode(aiNode* node, const aiScene* scene);
        
        /**
         * Processes an aiMesh to Mesh<Vertex>
         *
         * @param mesh aiMesh to be processed
         * @param scene aiScene
         * @return Mesh<Vertex>
         */
        Mesh<Vertex> processMesh(aiMesh* mesh, const aiScene* scene);
        
        /**
         * Loads the materials from aiMaterial of given type
         *
         * @param material aiMaterial to be processed
         * @param type Type of the textures
         */
        std::vector<Texture*> loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                  std::string typeName);
    
        
        std::vector<Mesh<Vertex>> m_Meshes; ///< all meshes of this model
        
    };
}

#endif /* Model_h */
