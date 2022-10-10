/*
 *   Fragment Shader
 *
 *   CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 410 core

// all uniform inputs

// all varying inputs
in vec3 theColor;


// all fragment outputs
out vec4 fragColorOut;


void main() {
    //*****************************************
    //******* Final Color Calculations ********
    //*****************************************

    fragColorOut = vec4(theColor, 1);

}
