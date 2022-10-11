//
// Created by Jimmy Truong on 10/10/22.
//

#include "Clutch.hpp"

Clutch::Clutch() {
    //instantiate model
    _model = new CSCI441::ModelLoader();
    _limbs = new CSCI441::ModelLoader();
    _position = glm::vec3(0);
    _forward = glm::vec3(0,0,1);
    _strafeSpeed = 0;
    _walkSpeed = 0;
}

void Clutch::initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
    _model->loadModelFile( "models/clutchnorris.obj" );
    _model->setAttributeLocations(posAttr, normAttr, texAttr);
//    _limbs->loadModelFile( "models/clutchLimbs.obj" );
//    _limbs->setAttributeLocations(posAttr, normAttr, texAttr);
}

void Clutch::updatePosition(bool forward) {
    if (forward) {_position = _position + _forward;}
    else {_position = _position - _forward;}
}

void Clutch::changeForward() {
    GLfloat rads = (M_PI / 180.0f) * _playerAngle;
    _forward = glm::normalize(-glm::vec3(sin(-rads), 0, cos(-rads))) * _walkSpeed;
}
