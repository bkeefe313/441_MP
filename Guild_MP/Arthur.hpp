
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

};


#endif //A3_ARTHUR_HPP
