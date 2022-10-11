
#version 410 core

// all uniform inputs
uniform mat4 mvpMtx;

uniform float time;

// all attribute inputs
in vec3 vPosition;
in vec2 vTexCoord;
in vec3 vNormal;

// all varying outputs
out vec2 texCoord;

void main() {
    texCoord = vTexCoord;
    gl_Position = mvpMtx * vec4(vPosition, 1.0);
}