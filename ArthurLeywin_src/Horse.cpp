#include "Horse.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <CSCI441/objects.hpp>
#include <CSCI441/OpenGLUtils.hpp>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265
#endif

Horse::Horse(GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation) {
    _shaderProgramHandle = shaderProgramHandle;
    _shaderProgramUniformLocations.mvpMtx = mvpMtxUniformLocation;
    _shaderProgramUniformLocations.normalMtx = normalMtxUniformLocation;
    _shaderProgramUniformLocations.materialColor = materialColorUniformLocation;

    _rotateHorseAngle = M_PI / 2.0f;

    _bodyColor = glm::vec3(0.26f, 0.17f, 0.03f);
    _bodyScale = glm::vec3(0.75f, 2.0f, 1.0f);

    _legColor = glm::vec3(0.08f, 0.08f, 0.21f);
    _legScale = glm::vec3(1.0f, 0.25f, 0.25f);
    _legRotationAngle = 0.0f;
    _legRotationSpeed = M_PI / 32.0f;

    _headColor = glm::vec3(0.5f, 0.17f, 0.2f);
    _headScale = glm::vec3(0.75f, 1.0f, 0.75f);

    _neckColor = glm::vec3(0.26f, 0.17f, 0.03f);
    _neckScale = glm::vec3(0.50f, 0.75f, 0.50f);
    _neckRotateAngle = 0.785398f;

    _tailColor = glm::vec3(1.0f, 1.0f, 0.7f);
    _tailScale = glm::vec3(0.25f, 1.0f, 0.25f);
    _tailRotateAngle = 1.22173f;
    _tailRotationSpeed = M_PI / 32.0f;

    _maneColor = glm::vec3(1.0f, 1.0f, 1.0f);
    _maneScale = glm::vec3(0.25f, 0.75f, 0.25f);
    _maneRotateAngle = 0.785398f;
}

void Horse::drawHorse(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) {
    glUseProgram(_shaderProgramHandle);

    _initialRotate = M_PI / 2.0f;
    modelMtx = glm::rotate(modelMtx, -_rotateHorseAngle, CSCI441::Y_AXIS);
    modelMtx = glm::rotate(modelMtx, _initialRotate, CSCI441::Z_AXIS);


    _drawHorseBody(modelMtx, viewMtx, projMtx);
    _drawHorseLeg("topleft", modelMtx, viewMtx, projMtx);
    _drawHorseLeg("topright", modelMtx, viewMtx, projMtx);
    _drawHorseLeg("bottomleft", modelMtx, viewMtx, projMtx);
    _drawHorseLeg("bottomright", modelMtx, viewMtx, projMtx);
    _drawHorseNeck(modelMtx, viewMtx, projMtx);
    _drawHorseHead(modelMtx, viewMtx, projMtx);
    _drawHorseTail(modelMtx, viewMtx, projMtx);
    _drawHorseMane(modelMtx, viewMtx, projMtx);
}

void Horse::moveForward() {
//    horse leg movement
    _legRotationAngle += _legRotationSpeed;
    if (_legRotationAngle > M_PI / 4.0f) {
        _legRotationAngle = M_PI / 4.0f;
        _legRotationSpeed = -_legRotationSpeed;
    } else if (_legRotationAngle < -M_PI / 4.0f) {
        _legRotationAngle = -M_PI / 4.0f;
        _legRotationSpeed = -_legRotationSpeed;
    }
    setLegAngle(_legRotationAngle);
}

void Horse::moveBackward() {
//    horse leg movement
    _legRotationAngle -= _legRotationSpeed;
    if (_legRotationAngle > M_PI / 4.0f) {
        _legRotationAngle = M_PI / 4.0f;
        _legRotationSpeed = -_legRotationSpeed;
    } else if (_legRotationAngle < -M_PI / 4.0f) {
        _legRotationAngle = -M_PI / 4.0f;
        _legRotationSpeed = -_legRotationSpeed;
    }
    setLegAngle(_legRotationAngle);
}

void Horse::turnLeft() {
    _rotateHorseAngle -= M_PI / 62.0f;
    setRotateHorseAngle(_rotateHorseAngle);
}
//
void Horse::turnRight() {
    _rotateHorseAngle += M_PI / 62.0f;
    setRotateHorseAngle(_rotateHorseAngle);
}

void Horse::idle() {
//    swing horse tail
    _tailRotateAngle += M_PI / 64.0f;
    if (_tailRotateAngle > 1.22173f) {
        _tailRotateAngle = 1.22173f;
        _tailRotationSpeed = -_tailRotationSpeed;
    } else if (_tailRotateAngle < 0.785398f) {
        _tailRotateAngle = 0.785398f;
        _tailRotationSpeed = -_tailRotationSpeed;
    }
    setLegAngle(0.0f);
}

void Horse::_drawHorseBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::scale(modelMtx, _bodyScale);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_bodyColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_drawHorseLeg(std::string position, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    if (position == "topleft") {
        modelMtx = glm::translate(modelMtx, glm::vec3(-0.5f, -0.85f, 0.35f));
    } else if (position == "topright") {
        modelMtx = glm::translate(modelMtx, glm::vec3(-0.5f, -0.85f, -0.35f));
    } else if (position == "bottomleft") {
        modelMtx = glm::translate(modelMtx, glm::vec3(-0.5f, 0.85f, 0.35f));
    } else if (position == "bottomright") {
        modelMtx = glm::translate(modelMtx, glm::vec3(-0.5f, 0.85f, -0.35f));
    }
    modelMtx = glm::rotate(modelMtx, _legRotationAngle, CSCI441::Z_AXIS);
    modelMtx = glm::scale(modelMtx, _legScale);

    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_legColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_drawHorseNeck(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.5f, -1.0f, 0.0f));
    modelMtx = glm::rotate(modelMtx, _neckRotateAngle, CSCI441::Z_AXIS);
    modelMtx = glm::scale(modelMtx, _neckScale);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_neckColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_drawHorseHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::scale(modelMtx, _headScale);
    modelMtx = glm::translate(modelMtx, glm::vec3(0.75f, -1.5f, 0.0f));
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_headColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_drawHorseMane(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate(modelMtx, glm::vec3(0.65f, -0.80f, 0.0f));
    modelMtx = glm::rotate(modelMtx, _maneRotateAngle, CSCI441::Z_AXIS);
    modelMtx = glm::scale(modelMtx, _maneScale);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_maneColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_drawHorseTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    modelMtx = glm::translate(modelMtx, glm::vec3(-0.25f, 1.0f, 0.0f));
    modelMtx = glm::rotate(modelMtx, _tailRotateAngle, CSCI441::Z_AXIS);
    modelMtx = glm::scale(modelMtx, _tailScale);
    _computeAndSendMatrixUniforms(modelMtx, viewMtx, projMtx);
    glUniform3fv(_shaderProgramUniformLocations.materialColor, 1, &_tailColor[0]);
    CSCI441::drawSolidCube(1);
}

void Horse::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    glProgramUniformMatrix4fv( _shaderProgramHandle, _shaderProgramUniformLocations.mvpMtx, 1, GL_FALSE, &mvpMtx[0][0] );

    glm::mat3 normalMtx = glm::mat3( glm::transpose( glm::inverse( modelMtx )));
    glProgramUniformMatrix3fv( _shaderProgramHandle, _shaderProgramUniformLocations.normalMtx, 1, GL_FALSE, &normalMtx[0][0] );
}