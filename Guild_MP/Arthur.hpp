
#ifndef A3_ARTHUR_HPP
#define A3_ARTHUR_HPP

#include <CSCI441/ModelLoader.hpp>

class Arthur {
public:
    CSCI441::ModelLoader* _model;

    glm::vec3 _position;
    glm::vec3 _scale;
    glm::vec3 _rotation;

    Arthur() {
        _model = new CSCI441::ModelLoader();
        _position = glm::vec3(0,0,0);
        _scale = glm::vec3(0.5,0.5,0.5);
        _rotation = glm::vec3(0,0,0);
    }

    void initModel(GLint posAttrib, GLint normAttrib, GLint texAttrib) {
        _model->loadModelFile("models/Arthur.obj");
        _model->setAttributeLocations(posAttrib, normAttrib, texAttrib);
    }

//    Character Movement
    void updatePosition() {

    }


};


#endif //A3_ARTHUR_HPP
