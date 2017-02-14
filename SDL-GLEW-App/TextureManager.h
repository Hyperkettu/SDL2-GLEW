//
//  TextureManager.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 2/6/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef TextureManager_h
#define TextureManager_h

#include <GL/glew.h>

#include <unordered_map>

#include "Texture.h"

namespace Fox {
    
    /**
     * Texture manager for texture handling
     */
class TextureManager{
    
public:
    
    TextureManager(){}
    
    ~TextureManager(){
        freeAll();
    }
    
    /**
     * Access to singleton instance
     * 
     * @return texture manager
     */
    static TextureManager* Instance() {
        
        if (m_Singleton == nullptr)
            m_Singleton = new TextureManager;
        
        return m_Singleton;
    }
    
    /**
     * Frees all textures
     */
    void freeAll(){
    
        for(auto iter = m_Textures.begin(); iter != m_Textures.end(); iter++){
            delete iter->second;
        }
    
    }
    
    void freeTexture(GLuint Id){
    
    }
    
    /**
     * Loads a texture of type type from filePath
     *
     * @param filePath file path to texture
     * @param type Texture type
     * @return loaded texture
     */
    Texture* loadTexture(const GLchar* filePath, Texture::TextureType type){
    
        // if texture has not been loaded yet, load it
        if(m_Textures.find(filePath) == m_Textures.end()){
            
            Texture* texture = new Texture(filePath);
            texture->m_Type = type;
            m_Textures[filePath] = texture;
            return texture;
        }
        
        // otherwise get the desired texture
        return m_Textures[filePath];
    }
    
    /**
     * Accesses certain texture
     */
    Texture* getTexture(const GLchar* filePath){
        return m_Textures.find(filePath)->second;
    }
    
private:
    static TextureManager* m_Singleton; ///< texture manager
    
    std::unordered_map<const GLchar*, Texture*> m_Textures; ///< all textures
};

}
    
#endif /* TextureManager_h */
