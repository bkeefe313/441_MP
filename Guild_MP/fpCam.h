#ifndef A3_FPCAM_H
#define A3_FPCAM_H

#include <CSCI441/Camera.hpp>

class FPCam : public CSCI441::Camera{
public:
    void recomputeOrientation() final;

    void moveForward(GLfloat movementFactor) final;
    void moveBackward(GLfloat movementFactor) final;

    glm::vec3 playerPos;
    glm::vec3 playerForward;

    void updatePos(glm::vec3 pos);

    void updateForward(glm::vec3 forward);

};

#endif // A3_FPCAM_H
