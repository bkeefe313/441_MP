#ifndef ASSIGNMENT03_HORSE_H
#define ASSIGNMENT03_HORSE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

class Horse {
public:
    /// \desc creates a simple plane that gives the appearance of flight
    /// \param shaderProgramHandle shader program handle that the plane should be drawn using
    /// \param mvpMtxUniformLocation uniform location for the full precomputed MVP matrix
    /// \param normalMtxUniformLocation uniform location for the precomputed Normal matrix
    /// \param materialColorUniformLocation uniform location for the material diffuse color
    Horse( GLuint shaderProgramHandle, GLint mvpMtxUniformLocation, GLint normalMtxUniformLocation, GLint materialColorUniformLocation );

    /// \desc draws the model plane for a given MVP matrix
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    /// \note internally uses the provided shader program and sets the necessary uniforms
    /// for the MVP and Normal Matrices as well as the material diffuse color
    void drawHorse( glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx );

    GLfloat getRotateHorseAngle() { return _rotateHorseAngle; }
    void setRotateHorseAngle(GLfloat rotateHorseAngle) { _rotateHorseAngle = rotateHorseAngle; }

    GLfloat getLegAngle() { return _legRotationAngle; }
    void setLegAngle(GLfloat legRotationAngle) { _legRotationAngle = legRotationAngle; }

    void moveForward();
    void moveBackward();
    void turnRight();
    void turnLeft();
    void idle();

private:
    /// \desc handle of the shader program to use when drawing the horse
    GLuint _shaderProgramHandle;
    /// \desc stores the uniform locations needed for the plan information
    struct ShaderProgramUniformLocations {
        /// \desc location of the precomputed ModelViewProjection matrix
        GLint mvpMtx;
        /// \desc location of the precomputed Normal matrix
        GLint normalMtx;
        /// \desc location of the material diffuse color
        GLint materialColor;
    } _shaderProgramUniformLocations;

    /// \desc angle to rotate our horse at
    GLfloat _rotateHorseAngle;

    /// \desc color of horse's body
    glm::vec3 _bodyColor;
    /// \desc amount to scale the horse's body
    glm::vec3 _bodyScale;

    /// \desc color of horse's legs
    glm::vec3 _legColor;
    /// \desc amount to scale the horse's legs
    glm::vec3 _legScale;
    /// \desc amount to rotate the horse's legs
    GLfloat _legRotationAngle;
    /// \desc speed of rotation of the horse's legs
    GLfloat _legRotationSpeed;

    /// \desc color of horse's neck
    glm::vec3 _neckColor;
    /// \desc amount to scale the horse's neck
    glm::vec3 _neckScale;
    /// \desc amount to rotate the horse's neck
    GLfloat _neckRotateAngle;

    /// \desc color of horse's head
    glm::vec3 _headColor;
    /// \desc amount to scale the horse's head
    glm::vec3 _headScale;

    /// \desc color of horse's mane
    glm::vec3 _maneColor;
    /// \desc amount to scale the horse's mane
    glm::vec3 _maneScale;
    /// \desc amount to rotate the horse's mane
    GLfloat _maneRotateAngle;

    /// \desc color of horse's tail
    glm::vec3 _tailColor;
    /// \desc amount to scale the horse's tail
    glm::vec3 _tailScale;
    /// \desc amount to rotate the horse's tail
    GLfloat _tailRotateAngle;

    GLfloat _tailRotationSpeed;

    GLfloat _initialRotate;


    /// \desc draws just the horse's body
    /// \param modelMtx existing model matrix to apply to plane
    /// \param viewMtx camera view matrix to apply to plane
    /// \param projMtx camera projection matrix to apply to plane
    void _drawHorseBody(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;

    void _drawHorseLeg(std::string position, glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawHorseNeck(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawHorseHead(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawHorseMane(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;
    void _drawHorseTail(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx ) const;


    /// \desc precomputes the matrix uniforms CPU-side and then sends them
    /// to the GPU to be used in the shader for each vertex.  It is more efficient
    /// to calculate these once and then use the resultant product in the shader.
    /// \param modelMtx model transformation matrix
    /// \param viewMtx camera view matrix
    /// \param projMtx camera projection matrix
    void _computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const;


};


#endif //ASSIGNMENT03_HORSE_H
