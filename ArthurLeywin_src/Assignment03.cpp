#include "Assignment03.hpp"

#include <CSCI441/objects.hpp>

//*************************************************************************************
//
// Helper Functions

#ifndef M_PI
#define M_PI 3.14159265
#endif

/// \desc Simple helper function to return a random number between 0.0f and 1.0f.
GLfloat getRand() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

//*************************************************************************************
//
// Public Interface

Assignment03::Assignment03()
         : CSCI441::OpenGLEngine(4, 1,
                                 640, 480,
                                 "Assignment 03: The Cabin in the Woods") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

Assignment03::~Assignment03() {
    delete _freeCam;
}

void Assignment03::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;

            default: break; // suppress CLion warning
        }
    }
}

void Assignment03::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void Assignment03::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS && !_keys[GLFW_KEY_LEFT_SHIFT]) {
        // rotate the camera by the distance the mouse moved
        _freeCam->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                         (_mousePosition.y - currMousePosition.y) * 0.005f );
    }

//    Zoom in and out with Shift + Left Mouse Button
    if((_leftMouseButtonState == GLFW_PRESS && _keys[GLFW_KEY_LEFT_SHIFT]) || (_leftMouseButtonState == GLFW_PRESS && _keys[GLFW_KEY_RIGHT_SHIFT])) {
//        Zoom in
        if(currMousePosition.x > _mousePosition.x) {
            _freeCam->moveForward(_cameraSpeed.x);
        }
//        Zoom out
        else if(currMousePosition.x < _mousePosition.x) {
            _freeCam->moveBackward(_cameraSpeed.x);
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void Assignment03::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab05_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab05_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab05_engine_cursor_callback);

}

void Assignment03::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	        // clear the frame buffer to black
}

void Assignment03::_setupShaders() {
    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/lab05.v.glsl", "shaders/lab05.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.materialColor  = _lightingShaderProgram->getUniformLocation("materialColor");
    // TODO #3A: assign uniforms
    _lightingShaderUniformLocations.lightColor = _lightingShaderProgram->getUniformLocation("lightColor");
    _lightingShaderUniformLocations.lightDirection = _lightingShaderProgram->getUniformLocation("lightDirection");
    _lightingShaderUniformLocations.normalMatrix = _lightingShaderProgram->getUniformLocation("normalMatrix");
    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    // TODO #3B: assign attributes
    _lightingShaderAttributeLocations.vNormal        = _lightingShaderProgram->getAttributeLocation("vNormal");

}

void Assignment03::_setupBuffers() {

    // TODO #4: need to connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vNormal );

    // TODO #5: give the horse the normal matrix location
    _horse = new Horse(_lightingShaderProgram->getShaderProgramHandle(),
                       _lightingShaderUniformLocations.mvpMatrix,
                       -1,
                       _lightingShaderUniformLocations.materialColor);

    _createGroundBuffers();
    _createGridBuffers();
    _generateEnvironment();
}

void Assignment03::_createGroundBuffers() {
    // TODO #8: expand our struct
    struct Vertex {
        GLfloat x, y, z;
        GLfloat nx, ny, nz;
    };

    // TODO #9: add normal data
    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            { 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            {-1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f},
            { 1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGroundPoints = 4;

    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // TODO #10: hook up vertex normal attribute
    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vNormal);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Assignment03::_createGridBuffers() {
    int slices = 10;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec4> indices;

    for (int i = 0; i <= slices; i++) {
        for (int j = 0; j <= slices; j++) {
            float x = (float) j / slices;
            float y = 0;
            float z = (float) i / slices;
            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (int i =0; i < slices; i++) {
        for (int j = 0; j < slices; j++) {
            int row1 = i * (slices + 1);
            int row2 = (i + 1) * (slices + 1);
            indices.push_back(glm::vec4(row1 + j, row1 + j + 1, row1 + j + 1, row2 + j + 1));
            indices.push_back(glm::vec4(row2 + j + 1, row2 + j, row2 + j, row1 + j));
        }
    }

    glGenVertexArrays(1, &_gridVAO);
    glBindVertexArray(_gridVAO);

    GLuint gridVAO;
    glGenBuffers(1, &gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVAO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vPos);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vPos, 3, GL_FLOAT, GL_FALSE, sizeof(vertices), (void*)0);

    GLuint gridIBO;
    glGenBuffers(1, &gridIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::vec4), &indices[0], GL_STATIC_DRAW);


}

void Assignment03::_generateEnvironment() {
    //******************************************************************
    // parameters to make up our grid size and spacing, feel free to
    // play around with this
    const GLfloat GRID_WIDTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_LENGTH = WORLD_SIZE * 1.8f;
    const GLfloat GRID_SPACING_WIDTH = 1.0f;
    const GLfloat GRID_SPACING_LENGTH = 1.0f;
    // precomputed parameters based on above
    const GLfloat LEFT_END_POINT = -GRID_WIDTH / 2.0f - 5.0f;
    const GLfloat RIGHT_END_POINT = GRID_WIDTH / 2.0f + 5.0f;
    const GLfloat BOTTOM_END_POINT = -GRID_LENGTH / 2.0f - 5.0f;
    const GLfloat TOP_END_POINT = GRID_LENGTH / 2.0f + 5.0f;
    //******************************************************************
}

void Assignment03::_setupScene() {
    _freeCam = new CSCI441::ArcballCam();
//    _freeCam->setTheta( -M_PI / 3.0f );
//    _freeCam->setPhi( M_PI / 2.8f );
    _freeCam->setRadius( 10.0f );
    _freeCam->setPosition( glm::vec3(0.0f, 0.0f, 0.0f) );
    _freeCam->setLookAtPoint( glm::vec3(0.0f, 1.0f, 0.0f) );
    _freeCam->recomputeOrientation();
    _cameraSpeed = glm::vec2(0.15f, 0.01f);

    // TODO #6: set lighting uniforms
    glm::vec3 lightPosition = glm::vec3(-1, -1, -1);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    glProgramUniform3fv(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderUniformLocations.lightDirection,
            1,
            &lightPosition[0]);
    glProgramUniform3fv(
            _lightingShaderProgram->getShaderProgramHandle(),
            _lightingShaderUniformLocations.lightColor,
            1,
            &lightColor[0]);
}

//*************************************************************************************
//
// Engine Cleanup

void Assignment03::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
}

void Assignment03::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _horse;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void Assignment03::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // use our lighting shader program
    _lightingShaderProgram->useProgram();

    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(0.0f, 0.2f, 0.0f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);

    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);

    //// END DRAWING THE GROUND PLANE ////

    //// BEGIN DRAWING GRID ////
    // draw the grid
    glm::mat4 gridModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(gridModelMtx, viewMtx, projMtx);

    glm::vec3 gridColor(0.0f, 0.0f, 0.0f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &gridColor[0]);
    glBindVertexArray(_gridVAO);
    //// END DRAWING GRID ////

    //// BEGIN DRAWING THE HORSE ////
    glm::mat4 modelMtx(1.0f);
    // we are going to cheat and use our look at point to place our plane so that it is always in view
    modelMtx = glm::translate( modelMtx, _freeCam->getLookAtPoint() );
    // rotate the plane with our camera theta direction (we need to rotate the opposite direction so that we always look at the back)
//    modelMtx = glm::rotate( modelMtx, -_freeCam->getTheta(), CSCI441::Y_AXIS );
    // rotate the plane with our camera phi direction
//    modelMtx = glm::rotate( modelMtx,  _freeCam->getPhi(), CSCI441::X_AXIS );
    // draw our plane now
    _horse->drawHorse( modelMtx, viewMtx, projMtx );
    //// END DRAWING THE PLANE ////
}

void Assignment03::_updateScene() {
    // fly
    if( _keys[GLFW_KEY_SPACE] ) {
        // go backward if shift held down
        if( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
            _freeCam->moveBackward(_cameraSpeed.x);
        }
        // go forward
        else {
            _freeCam->moveForward(_cameraSpeed.x);
        }
    }

    if (_keys[GLFW_KEY_W]) {
        _horse->moveForward();
        if (_freeCam->getLookAtPoint().z <= WORLD_SIZE && _freeCam->getLookAtPoint().z >= -WORLD_SIZE && _freeCam->getLookAtPoint().x <= WORLD_SIZE && _freeCam->getLookAtPoint().x >= -WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(
                    0.1f * cos(_horse->getRotateHorseAngle()),
                    0.0f,
                    0.1f * sin(_horse->getRotateHorseAngle())));
            _freeCam->recomputeOrientation();
        }
        if (_freeCam->getLookAtPoint().z >= WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(0.0f,0.0f,-1.0f));
            _freeCam->recomputeOrientation();
        }
        if (_freeCam->getLookAtPoint().z <= -WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(0.0f,0.0f,1.0f));
            _freeCam->recomputeOrientation();
        }
        if (_freeCam->getLookAtPoint().x >= WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(-1.0f,0.0f,0.0f));
            _freeCam->recomputeOrientation();
        }
        if (_freeCam->getLookAtPoint().x <= -WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(1.0f,0.0f,0.0f));
            _freeCam->recomputeOrientation();
        }

    }
    if (_keys[GLFW_KEY_S]) {
        _horse->moveBackward();
        if (_freeCam->getLookAtPoint().z > -WORLD_SIZE && _freeCam->getLookAtPoint().z < WORLD_SIZE && _freeCam->getLookAtPoint().x > -WORLD_SIZE && _freeCam->getLookAtPoint().x < WORLD_SIZE) {
            _freeCam->setLookAtPoint(_freeCam->getLookAtPoint() + glm::vec3(
                    -0.1f * cos(_horse->getRotateHorseAngle()),
                    0.0f,
                    -0.1f * sin(_horse->getRotateHorseAngle())));
            _freeCam->recomputeOrientation();
        }
    }

//    turn horse with a and d
    if (_keys[GLFW_KEY_A]) {
        _horse->turnLeft();
    }
    if (_keys[GLFW_KEY_D]) {
        _horse->turnRight();
    }
}

void Assignment03::run() {
    //  This is our draw loop - all rendering is done here.  We use a loop to keep the window open
    //	until the user decides to close the window and quit the program.  Without a loop, the
    //	window will display once and then the program exits.
    while( !glfwWindowShouldClose(_window) ) {	        // check if the window was instructed to be closed
        glDrawBuffer( GL_BACK );				        // work with our back frame buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

        // Get the size of our framebuffer.  Ideally this should be the same dimensions as our window, but
        // when using a Retina display the actual window can be larger than the requested window.  Therefore,
        // query what the actual size of the window we are rendering to is.
        GLint framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize( _window, &framebufferWidth, &framebufferHeight );

        // update the viewport - tell OpenGL we want to render to the whole window
        glViewport( 0, 0, framebufferWidth, framebufferHeight );

        // set the projection matrix based on the window size
        // use a perspective projection that ranges
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 1000].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 1000.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _freeCam->getViewMatrix();

        // draw everything to the window
        _renderScene(viewMatrix, projectionMatrix);

        _updateScene();

        glfwSwapBuffers(_window);                       // flush the OpenGL commands and make sure they get rendered!
        glfwPollEvents();				                // check for any events and signal to redraw screen
    }
}

//*************************************************************************************
//
// Private Helper FUnctions

void Assignment03::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);

    // TODO #7: compute and send the normal matrix
    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderAttributeLocations.vNormal, normalMtx);
}

//*************************************************************************************
//
// Callbacks

void lab05_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (Assignment03*) glfwGetWindowUserPointer(window);
    if (action == GLFW_RELEASE) {
        engine->_horse->idle();
    }

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab05_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (Assignment03*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab05_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (Assignment03*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
