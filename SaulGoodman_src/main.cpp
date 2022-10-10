/*
 *  CSCI 441, Computer Graphics, Fall 2022
 *
 *  Project: lab04
 *  File: main.cpp
 *
 *  Description:
 *      This file contains the basic setup to work with GLSL shaders.
 *
 *  Author: Dr. Paone, Colorado School of Mines, 2022
 *
 */

#include "Lab04Engine.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

///*****************************************************************************
//
// Our main function
int main() {

    auto labEngine = new Lab04Engine();
    labEngine->initialize();
    if (labEngine->getError() == CSCI441::OpenGLEngine::OPENGL_ENGINE_ERROR_NO_ERROR) {
        labEngine->run();
    }
    labEngine->shutdown();
    delete labEngine;

	return EXIT_SUCCESS;
}
