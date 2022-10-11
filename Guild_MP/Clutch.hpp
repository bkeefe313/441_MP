#ifndef A3_CLUTCH_HPP
#define A3_CLUTCH_HPP

#ifndef M_PI
#define M_PI 3.14159265
#endif

#include <CSCI441/ModelLoader.hpp>

class Clutch {
public:
    CSCI441::ModelLoader* _model;
    CSCI441::ModelLoader* _limbs;
    glm::vec3 _position;
    glm::vec3 _forward;
    GLfloat _playerAngle;
    GLfloat _angle;
    GLfloat _limbAngle;

    Clutch();

    void initModel(GLint posAttr, GLint normAttr, GLint texAttr);

    void updatePosition(bool forward);

    void changeForward();

private:
    GLfloat _walkSpeed;
    GLfloat _strafeSpeed;
};


#endif //A3_CLUTCH_HPP
