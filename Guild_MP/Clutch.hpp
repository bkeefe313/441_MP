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
        _position = glm::vec3(0,0,0);
        _forward = glm::vec3(0,0,1);
    }

    void initModel(GLint posAttr, GLint normAttr, GLint texAttr) {
        _model->loadModelFile( "models/clutchnorris.obj" );
        _model->setAttributeLocations(posAttr, normAttr, texAttr);
    }

    
};


#endif //A3_CLUTCH_HPP
