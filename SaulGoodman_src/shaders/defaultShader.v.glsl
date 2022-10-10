/*
 *   Vertex Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs
uniform mat4 mvpMtx;
uniform vec3 matColor;

// all attribute inputs
in vec3 vPosition;

// all varying outputs
out vec3 theColor;


void main() {
    //*****************************************
    //********* Vertex Calculations  **********
    //*****************************************

    gl_Position = mvpMtx * vec4(vPosition, 1);


    theColor = matColor;

}