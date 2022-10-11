//
// Created by Ben Keefe on 10/6/22.
//

#ifndef LAB04_PLAYER_H
#define LAB04_PLAYER_H


#include <glm/ext/scalar_constants.hpp>
#include <GL/glew.h>
#include <CSCI441/ModelLoader.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <CSCI441/ShaderProgram.hpp>


class Saul {
public:
    CSCI441::ModelLoader* _model;
    CSCI441::ModelLoader* _limbs;
    glm::vec3 _position;
    glm::vec3 _forward;
    GLfloat _angle;
    GLfloat _limbAngle;

    Saul() {
        //instantiate models
        _model = new CSCI441::ModelLoader();
        _limbs = new CSCI441::ModelLoader();
        _position = glm::vec3(20,0,20);
        _forward = glm::vec3(0,0,1);
        _strafeSpeed = 0;
        _walkSpeed = 0;
    }

    /// \desc load model data and set attribute locations
    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile( "models/saul.obj" );
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
        _limbs->loadModelFile( "models/saul_limbs.obj" );
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

    void draw(glm::mat4 projMtx, glm::mat4 viewMtx, CSCI441::ShaderProgram* shader) {
        glm::mat4 mvpMtx = projMtx * viewMtx;
        glm::mat4 finalMtx = glm::scale(mvpMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        finalMtx = glm::translate(finalMtx, _position);
        finalMtx = glm::rotate(finalMtx, _angle, glm::vec3(0,1,0));
        finalMtx = glm::rotate(finalMtx, -glm::pi<GLfloat>()/2, glm::vec3(1,0,0));
        shader->useProgram();

        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(), shader->getUniformLocation("mvpMatrix"),
                                  1, GL_FALSE, &finalMtx[0][0]);
        _model->draw( shader->getShaderProgramHandle() );


        finalMtx = glm::rotate(finalMtx, _limbAngle, glm::vec3(1,0,0));
        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(), shader->getUniformLocation("mvpMatrix"),
                                  1, GL_FALSE, &finalMtx[0][0]);
        _limbs->draw( shader->getShaderProgramHandle() );

    }
private:
    GLfloat _walkSpeed;
    GLfloat _strafeSpeed;
};


#endif //LAB04_PLAYER_H
