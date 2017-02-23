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
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
        
        // check if evertything is loaded properly
        if(!scene) {
            std::cout << "No scene. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }
        
 /**      if(scene->mFlags | AI_SCENE_FLAGS_INCOMPLETE) {
            std::cout << "Scene incomplete. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }*/
        if(!scene->mRootNode){
            std::cout << "No root node. Assimp error: " << import.GetErrorString() << std::endl;
            return;
        }
        
        this->directory = path.substr(0, path.find_last_of('/'));
        
        // start processing from root node
        processNode(scene->mRootNode, scene);
    }
    
    void Model::processNode(aiNode* node , const aiScene* scene){
        
        // process all nodes meshes
        for(GLuint i = 0; i < node->mNumMeshes; i++){
        
            aiMesh* aMesh = scene->mMeshes[node->mMeshes[i]];
            Mesh<Vertex>* mesh = processMesh(aMesh, scene);
            m_Meshes.push_back(mesh);
        }
        
        // process children
        for(GLuint i = 0; i < node->mNumChildren; i++){
            processNode(node->mChildren[i], scene);
        }
    }
    
    Mesh<Vertex>* Model::processMesh(aiMesh* mesh, const aiScene* scene){
        
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        
        // processs vertices
        for(GLuint i = 0; i < mesh->mNumVertices; i++) {
            
            Vertex vertex;
            
            // process position
            glm::vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            vertex.m_Position = position;
            
            
            // process normals
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            vertex.m_Normal = normal;
           
            
            // does the mesh contain textures coordinates
            if(mesh->mTextureCoords[0]) {
                // process texture coordinates
                glm::vec2 uvs;
                uvs.x = mesh->mTextureCoords[0][i].x;
                uvs.y = mesh->mTextureCoords[0][i].y;
                vertex.m_TexCoords = uvs;
                
            }
            else {
                
                vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            
            vertices.push_back(vertex);
        }
        
        // process indices
        for(GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            
            for(GLuint j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
                
            }
        }
        
        Mesh<Vertex>* m = new Mesh<Vertex>(vertices, indices, GL_STATIC_DRAW);
        
       
        // process material
        if(mesh->mMaterialIndex >= 0) {
            
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture*> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::Diffuse);
            
            // add only first diffuse map
            if(diffuseMaps.size() > 0)
            m->addTexture(diffuseMaps[0]);
            
            std::vector<Texture*> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::Specular);
            
            // add only first specular map
            if(specularMaps.size() > 0)
                m->addTexture(specularMaps[0]);
            
            
        }
        
        return m;
    }
    
    std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                      Texture::TextureType texType) {
    
        std::vector<Texture*> textures;
        
        for(GLuint i = 0; i < material->GetTextureCount(type); i++)
        {
            aiString str;
            material->GetTexture(type, i, &str);
            
            std::string filename = std::string(str.C_Str());
            filename = directory + '/' + filename;
            
            const GLchar* filePath = filename.c_str();
            
            Texture* texture;
            TextureManager* textureManager = TextureManager::Instance();
            texture = textureManager->loadTexture(filePath, texType);
            textures.push_back(texture);
        }
        return textures;
    }
}
