#include "Player.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>

#ifndef M_PI
#define M_PI 3.14159265
#endif

Player::Player(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation ) {
    _wheelAngle = 0.0f;
    _wheelAngleRotationSpeed = M_PI / 16.0f;

    _shaderProgramHandle                            = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx           = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx        = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor    = materialColorUniformLocation;

    _rotateCarAngle = M_PI / 2.0f;

    _colorBody = glm::vec3( 0.0f, 0.0f, 1.0f );
    _scaleBody = glm::vec3( 2.0f, 0.5f, 1.0f );

    _colorNose = glm::vec3( 0.0f, 1.0f, 0.0f );
    _rotateNoseAngle = M_PI / 2.0f;

    _colorWheel = glm::vec3( 1.0f, 1.0f, 1.0f );
    _scaleWheel = glm::vec3( 1.1f, 1.0f, 0.025f );
    _transWheel = glm::vec3( 0.1f, 0.0f, 0.0f );

    _colorTail = glm::vec3( 1.0f, 1.0f, 1.0f );
}

void Player::drawPlayer(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) {
    glUseProgram( _shaderProgramHandle );

    modelMtx = glm::rotate( modelMtx, -_rotateCarAngle, CSCI441::Y_AXIS );
    modelMtx = glm::rotate( modelMtx, _rotateCarAngle, CSCI441::Z_AXIS );
    _drawCarBody(modelMtx, viewMtx, projMtx);
    _drawCarNose(modelMtx, viewMtx, projMtx);

    glm::mat4 modelMtxW1 = glm::rotate( modelMtx, _rotateCarAngle, CSCI441::Y_AXIS );
    modelMtxW1 = glm::translate( modelMtxW1, glm::vec3(0,0,0.1));
    _drawCarWheel(modelMtxW1, viewMtx, projMtx);

    glm::mat4 modelMtxW2 = glm::rotate( modelMtx, _rotateCarAngle, CSCI441::Y_AXIS );
    modelMtxW2 = glm::translate( modelMtxW2, glm::vec3(0,0,-0.1));
    _drawCarWheel(modelMtxW2, viewMtx, projMtx);

    glm::mat4 modelMtxW3 = glm::rotate( modelMtx, _rotateCarAngle, CSCI441::Y_AXIS );
    modelMtxW3 = glm::translate( modelMtxW3, glm::vec3(-0.2,0,0.1));
    _drawCarWheel(modelMtxW3, viewMtx, projMtx);

    glm::mat4 modelMtxW4 = glm::rotate( modelMtx, _rotateCarAngle, CSCI441::Y_AXIS );
    modelMtxW4 = glm::translate( modelMtxW4, glm::vec3(-0.2,0,-0.1));
    _drawCarWheel(modelMtxW4, viewMtx, projMtx);

    _drawCarTail(modelMtx, viewMtx, projMtx);        // the tail
}

void Player::moveForward() {
    _wheelAngle -= _wheelAngleRotationSpeed;
    if( _wheelAngle < 0.0f ) _wheelAngle += 2.0f * M_PI;
}

void Player::moveBackward() {
    _wheelAngle += _wheelAngleRotationSpeed;
    if( _wheelAngle > 2.0f * M_PI ) _wheelAngle -= 2.0f * M_PI;
}

void Player::_drawCarBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::scale( modelMtx, _scaleBody );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorBody[0]);

    CSCI441::drawSolidCube( 0.1 );
}

void Player::_drawCarNose(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    modelMtx = glm::rotate( modelMtx, _rotateNoseAngle, CSCI441::Z_AXIS );

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorNose[0]);

    CSCI441::drawSolidCone( 0.025, 0.3, 16, 16 );
}

void Player::_drawCarWheel(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    glm::mat4 modelMtx1 = glm::translate( modelMtx, _transWheel );
    modelMtx1 = glm::rotate( modelMtx1, _wheelAngle, CSCI441::X_AXIS );
    modelMtx1 = glm::scale( modelMtx1, _scaleWheel );

    _computeAndSendMatrixUniforms(modelMtx1, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorWheel[0]);

    CSCI441::drawSolidCube( 0.1 );

    glm::mat4 modelMtx2 = glm::translate( modelMtx, _transWheel );
    modelMtx2 = glm::rotate( modelMtx2, static_cast<GLfloat>(M_PI / 2.0f) + _wheelAngle, CSCI441::X_AXIS );
    modelMtx2 = glm::scale( modelMtx2, _scaleWheel );

    _computeAndSendMatrixUniforms(modelMtx2, viewMtx, projMtx);

    CSCI441::drawSolidCube( 0.1 );
}

void Player::_drawCarTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const {
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);

    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_colorTail[0]);

    CSCI441::drawSolidCone( 0.02, 0.1, 16, 16 );
}

void Player::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}
