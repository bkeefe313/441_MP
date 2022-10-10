#include "ArcBall.h"
#include <cmath>

void ArcBall::recomputeOrientation() {
    // TODO #2
    float x = _radius * sin(CSCI441::Camera::getTheta()) * sin(CSCI441::Camera::getPhi());
    float y = _radius * cos(CSCI441::Camera::getPhi());
    float z = -_radius * cos(CSCI441::Camera::getTheta()) * sin(CSCI441::Camera::getPhi());

    // TODO #3
    _direction = glm::vec3(x, y, z);
    CSCI441::Camera::setPosition(_direction + this->playerPos);
    CSCI441::Camera::setLookAtPoint(glm::vec3(0,0.05,0) + this->playerPos);
    CSCI441::Camera::computeViewMatrix();
}

void ArcBall::moveForward(GLfloat movementFactor) {
    if(_radius > 0.7){
        _radius -= movementFactor;
    }
    recomputeOrientation();
}

void ArcBall::moveBackward(GLfloat movementFactor) {
    _radius += movementFactor;
    recomputeOrientation();
}

void ArcBall::updatePos(glm::vec3 pos) {
    this->playerPos = pos;
}