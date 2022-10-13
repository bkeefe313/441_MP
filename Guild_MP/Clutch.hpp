#ifndef A3_CLUTCH_HPP
#define A3_CLUTCH_HPP

#ifndef M_PI
#define M_PI 3.14159265
#endif

#include <CSCI441/ModelLoader.hpp>
#include "Character.hpp"

class Clutch : public Character {
public:
    CSCI441::ModelLoader* _model;

    Clutch() {
        //instantiate model
        _model = new CSCI441::ModelLoader();
        _position = glm::vec3(10,0,0);
        _forward = glm::vec3(0,0,1);
    }

    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile( "models/clutchnorris.obj" );
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
    }

    void draw(glm::mat4 projMtx, glm::mat4 viewMtx, CSCI441::ShaderProgram* shader) {
        glm::mat4 mvpMtx = projMtx * viewMtx;

        glm::mat4 finalMtx = glm::translate(mvpMtx, _position);
        finalMtx = glm::rotate(finalMtx,  _playerAngle, glm::vec3(0,1,0));

        shader->useProgram();

        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(),
                                  shader->getUniformLocation("mvpMatrix"),
                                  1,
                                  GL_FALSE, &finalMtx[0][0]);
        _model->draw(shader->getShaderProgramHandle());
    }
};


#endif //A3_CLUTCH_HPP
