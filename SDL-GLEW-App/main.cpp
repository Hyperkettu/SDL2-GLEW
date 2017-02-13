//
//  main.cpp
//  SDL_GLEW_Test
//
//  Created by Olli Kettunen on 1/22/17.
//  Copyright © 2017 Olli Kettunen. All rights reserved.
//

#include <iostream>

#include <SDL2/SDL.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL_opengl.h>

#include "Application.h"

const GLint WIDTH = 800, HEIGHT = 600; // dimensions of the window

int main(int argc, const char * argv[]) {
    
    // initialize application
    Fox::Application app("SDL2 GLEW Demo Game", WIDTH, HEIGHT, false);
    
    return EXIT_SUCCESS;
}
