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
#include "Character.hpp"


class Saul : public Character {
public:
    CSCI441::ModelLoader* _model;
    CSCI441::ModelLoader* _limbs;
    GLfloat _limbAngle;

    Saul() {
        //instantiate models
        _model = new CSCI441::ModelLoader();
        _limbs = new CSCI441::ModelLoader();
        _position = glm::vec3(20,0,20);
        _forward = glm::vec3(0,0,1);
    }

    /// \desc load model data and set attribute locations
    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile( "models/saul.obj" );
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
        _limbs->loadModelFile( "models/saul_limbs.obj" );
        _limbs->setAttributeLocations(posAttr, normAttr, texAttr);
    }


    void draw(glm::mat4 projMtx, glm::mat4 viewMtx, CSCI441::ShaderProgram* shader) {
        glm::mat4 mvpMtx = projMtx * viewMtx;

        glm::mat4 finalMtx = glm::translate(mvpMtx, _position);
        finalMtx = glm::rotate(finalMtx, _playerAngle, glm::vec3(0,1,0));
        finalMtx = glm::rotate(finalMtx, -glm::pi<GLfloat>()/2, glm::vec3(1,0,0));
        finalMtx = glm::scale(finalMtx, glm::vec3(0.2f, 0.2f, 0.2f));
        shader->useProgram();

        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(), shader->getUniformLocation("mvpMatrix"),
                                  1, GL_FALSE, &finalMtx[0][0]);
        _model->draw( shader->getShaderProgramHandle() );

        _limbAngle += 0.01;
        finalMtx = glm::scale(finalMtx, glm::vec3(abs(sin(_limbAngle))));
        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(), shader->getUniformLocation("mvpMatrix"),
                                  1, GL_FALSE, &finalMtx[0][0]);
        _limbs->draw( shader->getShaderProgramHandle() );

    }
};


#endif //LAB04_PLAYER_H
