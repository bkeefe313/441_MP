#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
// TODO #D: add normal matrix
uniform mat3 normMatrix;

// point light uniforms
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

// directional light uniforms
uniform vec3 dirLightDir;
uniform vec3 dirLightColor;

//spotlight uniforms
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform float spotLightAngle;
uniform vec3 spotLightColor;

//camera position uniform
uniform vec3 camPos;

in vec2 vTexCoord;
out vec2 texCoord;

// attribute inputs
layout(location = 0) in vec3 vPos;      // the position of this specific vertex in object space
// TODO #C: add vertex normal
in vec3 vertNorm;

// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // TODO #B: computer Light vectors
    vec3 dirLightRefl = normalize(-dirLightDir);
    vec3 pointLightRefl = normalize(pointLightPos - vPos);
    vec3 spotLightRefl = normalize(spotLightPos - vPos);
    if(abs(acos(dot(spotLightRefl, normalize(spotLightDir)))) > spotLightAngle)
    spotLightRefl = vec3(0);

    //compute attenutations
    float pointAttenuation = length(vPos - pointLightPos);
    float spotAttenuation = length(vPos - spotLightPos);

    //compute view dir
    vec3 viewDir = normalize(camPos - vPos);

    // TODO #E: transform normal vector
    vec3 transVert = normMatrix * vertNorm;

    // TODO #F: perform diffuse calculations
    vec3 i_d =  (pointLightColor * max(dot(pointLightRefl, transVert), 0)/pointAttenuation) +
                dirLightColor * max(dot(dirLightRefl, transVert), 0) +
                (spotLightColor * max(dot(spotLightRefl, transVert), 0)/spotAttenuation);

    // perform specular calculations
    vec3 i_s =  (pointLightColor * max(dot(pointLightRefl, viewDir), 0)/pointAttenuation) +
                dirLightColor * max(dot(dirLightRefl, viewDir), 0) +
                (spotLightColor * max(dot(spotLightRefl, viewDir), 0)/spotAttenuation);

    //perform ambient calculation
    vec3 i_a = vec3(1) / 3;

    //add together components
    vec3 i_tot = i_d + i_s + i_a;

    // TODO #G: assign the color for this vertex
    color = i_tot;
    texCoord = vTexCoord;
}