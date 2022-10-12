
#ifndef A3_CHARACTER_HPP
#define A3_CHARACTER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <CSCI441/ShaderProgram.hpp>
#include <CSCI441/ModelLoader.hpp>

class Character {
public:
    glm::vec3 _position;
    glm::vec3 _forward;
    GLfloat _walkSpeed = 0.07;
    GLfloat _playerAngle = 0;
    glm::vec3 _scale;
    glm::vec3 _color;
    glm::vec3 _rotation;
    glm::vec3 _characterRotation;

    void updatePosition(bool forward){
        if (forward) {_position += (_forward * _walkSpeed);}
        else {_position -= (_forward * _walkSpeed);}
    }

//    void changeForward(){
//        glm::vec3 posZ = glm::vec3(0, 0, 1);
//        GLfloat rads = (M_PI / 180.0f) * _playerAngle;
//        _playerAngle = (-(float) dirOfCross(_forward, posZ)) * acos(glm::dot(_forward, posZ));
//
//        glm::normalize(-glm::vec3(sin(rads), 0, cos(rads)))
//    }

    static int dirOfCross(glm::vec3 a, glm::vec3 b) {
        return glm::cross(a, b).y > 0 ? 1 : -1;
    }

    /// \desc change the movement direction from camera position
    void updateAngle(GLfloat speed) {
        _playerAngle -= speed;
        if(_playerAngle > 2.0f * M_PI) {
            _playerAngle = 0.01;
        } else if(_playerAngle < 0) {
            _playerAngle = M_PI * 1.99f;
        }
        _forward = glm::normalize(glm::vec3(sin(_playerAngle), 0, cos(_playerAngle)));
    }

private:
};

#endif //A3_CHARACTER_HPP
