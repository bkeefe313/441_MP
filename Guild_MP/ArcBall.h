#ifndef A3_ARCBALL_H
#define A3_ARCBALL_H

#include <CSCI441/Camera.hpp>

class ArcBall : public CSCI441::Camera{
public:
    void recomputeOrientation() final;

    void moveForward(GLfloat movementFactor) final;
    void moveBackward(GLfloat movementFactor) final;

    glm::vec3 playerPos;

    void updatePos(glm::vec3 pos);

};

#endif // A3_ARCBALL_H
