//
// Created by Ben Keefe on 10/6/22.
//

#ifndef LAB04_PLAYER_H
#define LAB04_PLAYER_H


#include <glm/ext/scalar_constants.hpp>
#include "include/CSCI441/ModelLoader.hpp"


/// \desc essntially an object that is controlled by the user, contains all necessary data to render and move around
class Player {
public:
    CSCI441::ModelLoader* _model;
    CSCI441::ModelLoader* _limbs;
    glm::vec3 _position;
    glm::vec3 _forward;
    GLfloat _angle;
    GLfloat _limbAngle;

    Player() {
        //instantiate model
        _model = new CSCI441::ModelLoader();
        _limbs = new CSCI441::ModelLoader();
        _position = glm::vec3(0);
        _forward = glm::vec3(0,0,1);
        _strafeSpeed = 0;
        _walkSpeed = 0;
    }

    /// \desc load model data and set attribute locations
    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile( "models/saul.obj" );
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
        _limbs->loadModelFile( "models/limbs.obj" );
        _limbs->setAttributeLocations(posAttr, normAttr, texAttr);
    }

    static int dirOfCross(glm::vec3 a, glm::vec3 b) {
        return glm::cross(a, b).y > 0 ? 1 : -1;
    }

    /// \desc move player based on current strafe and walk speed
    void updatePosition() {
        glm::vec3 posZ = glm::vec3(0,0,1);
        if(abs(_strafeSpeed) > 0 || abs(_walkSpeed) > 0) {
            _angle = (-(float) dirOfCross(_forward, posZ)) * acos(glm::dot(_forward, posZ));
            _limbAngle += 0.01f;
            if(_limbAngle >= 6.28) {
                _limbAngle = 0;
            }
        } else {
            _limbAngle = 0;
        }
        _position += (_strafeSpeed * (glm::cross(_forward, glm::vec3(0, 1, 0)))) + (_walkSpeed * _forward);
    }

    /// \desc update walk speed to a value (pos = +z, neg = -z)
    void walk(GLfloat speed) {
        _walkSpeed = speed;
    }

    /// \desc update strafe speed to a value (pos = +x, neg = -x)
    void strafe(GLfloat speed) {
        _strafeSpeed = speed;
    }

    /// \desc change the movement direction from camera position
    void updateDirection(glm::vec3 v) {
        _forward = _position - v;
        _forward.y = 0;
        _forward = glm::normalize(_forward);
    }
private:
    GLfloat _walkSpeed;
    GLfloat _strafeSpeed;
};


#endif //LAB04_PLAYER_H
