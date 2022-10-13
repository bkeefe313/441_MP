
#ifndef A3_ARTHUR_HPP
#define A3_ARTHUR_HPP

#include <CSCI441/ModelLoader.hpp>
#include "Character.hpp"

class Arthur : public Character {
public:
    CSCI441::ModelLoader* _model;
    Arthur() {
        _model = new CSCI441::ModelLoader();
        _position = glm::vec3(0,0,0);
        _forward = glm::vec3(0,0,1);
    }

    void initModel(GLint posAttrib, GLint normAttrib, GLint texAttrib) {
        _model->loadModelFile("models/Arthur.obj");
        _model->setAttributeLocations(posAttrib, normAttrib, texAttrib);
    }

    void draw(glm::mat4 projMtx, glm::mat4 viewMtx, CSCI441::ShaderProgram* shader) {
        glm::mat4 mvpMtx = projMtx * viewMtx;

        glm::mat4 finalMtx = glm::translate(mvpMtx, _position);
        glm::vec3 _scale = glm::vec3(0.5,0.5,0.5);
        finalMtx = glm::rotate(finalMtx, _playerAngle, glm::vec3(0,1,0));
        finalMtx = glm::scale( finalMtx, _scale);

        shader->useProgram();

        glProgramUniformMatrix4fv(shader->getShaderProgramHandle(),
                                  shader->getUniformLocation("mvpMatrix"),
                                  1,
                                  GL_FALSE, &finalMtx[0][0]);
        _model->draw(shader->getShaderProgramHandle());
    }

};


#endif //A3_ARTHUR_HPP
