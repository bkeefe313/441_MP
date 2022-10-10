#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <GL/glew.h>

#include <glm/glm.hpp>

class Player {
public:
    /// \param shaderProgramHandle shader program handle that the car should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Player(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation );

    void drawPlayer( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    void moveForward();
    void moveBackward();

private:
    /// \desc current angle of rotation for the wheel
    GLfloat _wheelAngle;
    /// \desc one rotation step
    GLfloat _wheelAngleRotationSpeed;

    /// \desc handle of the shader program to use when drawing the car
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;

    /// \desc angle to rotate our car at
    GLfloat _rotateCarAngle;

    /// \desc color the Car's body
    glm::vec3 _colorBody;
    /// \desc amount to scale the Car's body by
    glm::vec3 _scaleBody;

    /// \desc color the Car's nose
    glm::vec3 _colorNose;
    /// \desc amount to rotate the Car's nose by
    GLfloat _rotateNoseAngle;

    /// \desc color the Car's wheel
    glm::vec3 _colorWheel;
    /// \desc amount to scale the Car's Wheel by
    glm::vec3 _scaleWheel;
    /// \desc amount to translate the Car's Wheel by
    glm::vec3 _transWheel;

    /// \desc color the Car's tail
    glm::vec3 _colorTail;

    /// \desc draws just the Car's body
    /// \param modelMtx existing model matrix to apply to Car
    /// \param viewMtx camera view matrix to apply to Car
    /// \param projMtx camera projection matrix to apply to Car
    void _drawCarBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc draws the nose of the car
    /// \param modelMtx existing model matrix to apply to Car
    /// \param viewMtx camera view matrix to apply to Car
    /// \param projMtx camera projection matrix to apply to Car
    void _drawCarNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the two Wheel pieces
    /// \param modelMtx existing model matrix to apply to Car
    /// \param viewMtx camera view matrix to apply to Car
    /// \param projMtx camera projection matrix to apply to Car
    void _drawCarWheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    /// \desc draws the tail of the car
    /// \param modelMtx existing model matrix to apply to Car
    /// \param viewMtx camera view matrix to apply to Car
    /// \param projMtx camera projection matrix to apply to Car
    void _drawCarTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;
};


#endif //PLAYER_HPP
