
#include "fpCam.h"
#include <cmath>

void FPCam::recomputeOrientation() {
    // TODO #2
    float x = _radius * sin(CSCI441::Camera::getTheta()) * sin(CSCI441::Camera::getPhi());
    float y = _radius * cos(CSCI441::Camera::getPhi());
    float z = -_radius * cos(CSCI441::Camera::getTheta()) * sin(CSCI441::Camera::getPhi());

    _direction = glm::vec3(x, y, z);
    //set position to target player position
    CSCI441::Camera::setPosition(this->playerPos);
    CSCI441::Camera::setLookAtPoint(_position + this->playerForward);
    CSCI441::Camera::computeViewMatrix();
}

void FPCam::moveForward(GLfloat movementFactor) {
    if(_radius > 3){
        _radius -= movementFactor;
    }
    recomputeOrientation();
}

void FPCam::moveBackward(GLfloat movementFactor) {
    _radius += movementFactor;
    recomputeOrientation();
}

void FPCam::updatePos(glm::vec3 pos) {
    this->playerPos = pos;
}

void FPCam::updateForward(glm::vec3 forward) {
    this->playerForward = forward;
}