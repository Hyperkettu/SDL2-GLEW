//
//  Model.cpp
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/13/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include "Model.h"

namespace Fox {
    
    void Model::loadModel(std::string path){
    
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
        
        // check if evertything is loaded properly
        if(!scene || scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Assimp error: " << import.GetErrorString() << std::endl;
        }
        
        // start processing from root node
        processNode(scene->mRootNode, scene);
    }
    
    void Model::processNode(aiNode* node , const aiScene* scene){
    
    }
    
    Mesh<Vertex> Model::processMesh(aiMesh* mesh, const aiScene* scene){
        return Mesh<Vertex>();
    }
    
    std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                      std::string typeName) {
    
        return std::vector<Texture*>();
    }
}
