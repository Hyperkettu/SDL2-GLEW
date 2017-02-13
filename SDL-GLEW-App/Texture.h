//
//  Texture.h
//  SDL-GLEW-App
//
//  Created by Olli Kettunen on 1/28/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#ifndef Texture_h
#define Texture_h

#include <iostream>

#include <GL/glew.h>

#include <SDL2_image/SDL_image.h>

namespace Fox {
  
    class Texture {
        
    public:
        
        enum TextureType
        {
            Diffuse = 0,    // Diffuse color map.
            Specular,       // Specular map
            Alpha,          // Alpha map (green = opacity).
            Displacement,   // Displacement map (green = height).
            Normal,         // Tangent-space normal map.
            Environment,    // Environment map (spherical coordinates).
            
            TextureType_Max
        };
        
        Texture(){}
        
        /**
         * Destroy loaded texture
         */
        ~Texture(){
            std::cout << "delete " << m_Id << std::endl;
            glDeleteTextures(1, &m_Id);
            
        }
        
        static SDL_Surface* readTexture(const GLchar* filePath){
            
            SDL_Surface* image;
            
            // load image from file
            image = IMG_Load(filePath);
            
            // if image loading failed
            if(!image) {
                std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
            }
            return image;
        }
        
        /**
         * Loads the texture from file path
         *
         * @param filePath File path
         */
        void load(const GLchar* filePath) {
        
            SDL_Surface* image;
            
            // load image from file
            image = IMG_Load(filePath);
            
            // if image loading failed
            if(!image) {
                std::cout << "IMG_Load: " << IMG_GetError() << std::endl;
            }
            
            // create texture
            glGenTextures(1, &m_Id);
            glBindTexture(GL_TEXTURE_2D, m_Id);
            
            GLint mode = GL_RGB;
            
            if(image->format->BytesPerPixel == 4) {
                mode = GL_RGBA;
            }
            
            // set parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            
            // note BGRA instead of RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, mode, image->w, image->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, image->pixels);
            
            // generate mipmaps
            glGenerateMipmap(GL_TEXTURE_2D);
            
          //  SDL_FreeSurface(image);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        
        /**
         * Loads this texture from a file path 
         *
         * @param filePath File path to texture file
         */
        Texture(const GLchar* filePath) {
            
            load(filePath);
            std::cout << "load " << m_Id << std::endl;
        }
        
        GLuint m_Id; ///< texture id
        TextureType m_Type; ///< texture type
    };
}


#endif /* Texture_h */
