#include "A3Engine.hpp"

#include <CSCI441/objects.hpp>
#include <cmath>
#include <iostream>

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

A3Engine::A3Engine()
         : CSCI441::OpenGLEngine(4, 1,
                                 640, 480,
                                 "A3") {

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

A3Engine::~A3Engine() {
    delete _arcBall;
}

void A3Engine::handleKeyEvent(GLint key, GLint action) {
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

void A3Engine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void A3Engine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        // rotate the camera by the distance the mouse moved
        _arcBall->rotate((currMousePosition.x - _mousePosition.x) * 0.005f,
                         (_mousePosition.y - currMousePosition.y) * 0.005f );
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

//*************************************************************************************
//
// Engine Setup

void A3Engine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab05_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab05_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab05_engine_cursor_callback);
}

void A3Engine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                        // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	    // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	        // clear the frame buffer to black
}

void A3Engine::_setupShaders() {
    _texShaderProgram = new CSCI441::ShaderProgram(
            "shaders/textureShader.v.glsl",
            "shaders/textureShader.f.glsl");

    // assign uniforms
    _textureShaderUniformLocations.mvpMatrix = _texShaderProgram->getUniformLocation("mvpMatrix");
    _textureShaderUniformLocations.pointLightColor = _texShaderProgram->getUniformLocation("pointLightColor");
    _textureShaderUniformLocations.pointLightPos = _texShaderProgram->getUniformLocation("pointLightPos");
    _textureShaderUniformLocations.spotLightPos = _texShaderProgram->getUniformLocation("spotLightPos");
    _textureShaderUniformLocations.spotLightDir = _texShaderProgram->getUniformLocation("spotLightDir");
    _textureShaderUniformLocations.spotLightAngle = _texShaderProgram->getUniformLocation("spotLightAngle");
    _textureShaderUniformLocations.spotLightColor = _texShaderProgram->getUniformLocation("spotLightColor");
    _textureShaderUniformLocations.dirLightDir = _texShaderProgram->getUniformLocation("dirLightDir");
    _textureShaderUniformLocations.dirLightColor = _texShaderProgram->getUniformLocation("dirLightColor");
    _textureShaderUniformLocations.normMatrix = _texShaderProgram->getUniformLocation("normMatrix");
    _textureShaderUniformLocations.camPos = _texShaderProgram->getUniformLocation("camPos");

    _textureShaderAttributeLocations.vPos = _texShaderProgram->getAttributeLocation("vPos");
    _textureShaderAttributeLocations.vTexCoord = _texShaderProgram->getAttributeLocation("vTexCoord");
    _textureShaderAttributeLocations.vertNorm = _texShaderProgram->getAttributeLocation("vertNorm");

    _lightingShaderProgram = new CSCI441::ShaderProgram("shaders/lab05.v.glsl", "shaders/lab05.f.glsl" );
    _lightingShaderUniformLocations.mvpMatrix      = _lightingShaderProgram->getUniformLocation("mvpMatrix");
    _lightingShaderUniformLocations.materialColor  = _lightingShaderProgram->getUniformLocation("materialColor");
    // TODO #3A: assign uniforms
    _lightingShaderUniformLocations.pointLightColor = _lightingShaderProgram->getUniformLocation("pointLightColor");
    _lightingShaderUniformLocations.pointLightPos = _lightingShaderProgram->getUniformLocation("pointLightPos");
    _lightingShaderUniformLocations.spotLightPos = _lightingShaderProgram->getUniformLocation("spotLightPos");
    _lightingShaderUniformLocations.spotLightDir = _lightingShaderProgram->getUniformLocation("spotLightDir");
    _lightingShaderUniformLocations.spotLightAngle = _lightingShaderProgram->getUniformLocation("spotLightAngle");
    _lightingShaderUniformLocations.spotLightColor = _lightingShaderProgram->getUniformLocation("spotLightColor");
    _lightingShaderUniformLocations.dirLightDir = _lightingShaderProgram->getUniformLocation("dirLightDir");
    _lightingShaderUniformLocations.dirLightColor = _lightingShaderProgram->getUniformLocation("dirLightColor");
    _lightingShaderUniformLocations.normMatrix = _lightingShaderProgram->getUniformLocation("normMatrix");
    _lightingShaderUniformLocations.camPos = _lightingShaderProgram->getUniformLocation("camPos");

    _lightingShaderAttributeLocations.vPos         = _lightingShaderProgram->getAttributeLocation("vPos");
    // TODO #3B: assign attributes
    _lightingShaderAttributeLocations.vertNorm = _lightingShaderProgram->getAttributeLocation("vertNorm");

}

void A3Engine::_setupBuffers() {
    // TODO #4: need to connect our 3D Object Library to our shader
    CSCI441::setVertexAttributeLocations( _textureShaderAttributeLocations.vPos, _textureShaderAttributeLocations.vertNorm);

    //// Bring in Objects
    _arthur = new Arthur();
    _arthur->initModel(_texShaderProgram->getAttributeLocation("vPos"),
                       _textureShaderAttributeLocations.vertNorm,
                       _texShaderProgram->getAttributeLocation("vTexCoord"));


    _clutch = new Clutch();
    _clutch->initModel(_texShaderProgram->getAttributeLocation("vPos"),
                       _textureShaderAttributeLocations.vertNorm,
                       _texShaderProgram->getAttributeLocation("vTexCoord"));

    _saul = new Saul();
    _saul->initModel(_texShaderProgram->getAttributeLocation("vPos"),
                     _textureShaderAttributeLocations.vertNorm,
                     _texShaderProgram->getAttributeLocation("vTexCoord"));

    CSCI441::setVertexAttributeLocations( _lightingShaderAttributeLocations.vPos, _lightingShaderAttributeLocations.vertNorm);

    // OG car model
    _player = new Player(_lightingShaderProgram->getShaderProgramHandle(),
                         _lightingShaderUniformLocations.mvpMatrix,
                         _lightingShaderUniformLocations.normMatrix,
                         _lightingShaderUniformLocations.materialColor);

    _createGroundBuffers();
    _generateEnvironment();
}

void A3Engine::_createGroundBuffers() {
    // TODO #8: expand our struct
    struct Vertex {
        GLfloat x, y, z;
        GLfloat xNorm, yNorm, zNorm;
    };

    // TODO #9: add normal data
    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            { 1.0f, 0.0f, -1.0f,0.0f, 1.0f, 0.0f},
            {-1.0f, 0.0f,  1.0f,0.0f, 1.0f, 0.0f},
            { 1.0f, 0.0f,  1.0f,0.0f, 1.0f, 0.0f}
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
    glEnableVertexAttribArray(_lightingShaderAttributeLocations.vertNorm);
    glVertexAttribPointer(_lightingShaderAttributeLocations.vertNorm, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)4);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void A3Engine::_generateEnvironment() {
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

    srand( time(0) );                                                   // seed our RNG

    // psych! everything's on a grid.
    for(int i = LEFT_END_POINT; i < RIGHT_END_POINT; i += GRID_SPACING_WIDTH) {
        for(int j = BOTTOM_END_POINT; j < TOP_END_POINT; j += GRID_SPACING_LENGTH) {
            // don't just draw a building ANYWHERE.
            if( i % 2 && j % 2 && getRand() < 0.2f ) {
                // translate to spot
                glm::mat4 transToSpotMtx = glm::translate( glm::mat4(1.0), glm::vec3(i, 0.0f, j) );

                // compute random height
                GLdouble height = powf(getRand(), 2.5)*10 + 1;
                // scale to building size
                glm::mat4 scaleToHeightMtx = glm::scale( glm::mat4(1.0), glm::vec3(1, height, 1) );

                // translate up to grid
                glm::mat4 transToHeight = glm::translate( glm::mat4(1.0), glm::vec3(0, height/2.0f, 0) );

                // compute full model matrix
                glm::mat4 modelMatrix = transToHeight * scaleToHeightMtx * transToSpotMtx;

                // compute random color
                glm::vec3 color( getRand(), getRand(), getRand() );
                // store building properties
                BuildingData currentBuilding = {modelMatrix, color};
                _buildings.emplace_back( currentBuilding );
            }
        }
    }
}

void A3Engine::_setupScene() {
//    Original Free cam implementation
//    _freeCam = new CSCI441::FreeCam();
//    _freeCam->setPosition( glm::vec3(60.0f, 40.0f, 30.0f) );
//    _freeCam->setTheta( -M_PI / 3.0f );
//    _freeCam->setPhi( M_PI / 2.8f );
//    _freeCam->recomputeOrientation();

    playerAngle = 0.0f;
    playerDirection = glm::normalize(-glm::vec3(0, 0, 0.07)) * 0.1f;
    _arcBall = new ArcBall();
    _arcBall->setPosition( glm::vec3(0.0f, 1.0f, 0.0f) );
    _arcBall->setLookAtPoint( glm::vec3(0.0f, 0.05f, 0.0f) );
    playerPos = _arcBall->getLookAtPoint();
    _arcBall->setTheta( -M_PI / 3.0f );
    _arcBall->setPhi( M_PI / 2.8f );
    _arcBall->setRadius(1);

    _arcBall->recomputeOrientation();

    _cameraSpeed = glm::vec2(0.25f, 0.02f);

    // TODO #6: set lighting uniforms
    _pointLight = new Light(glm::vec3(0, 100, 0), glm::vec3(1, 1, 1));
    _dirLight = new Light(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), DIRECTIONAL);
    _spotlight = new Light(glm::vec3(100, 100, -100), glm::vec3(-1, -1, 1), 20, glm::vec3(1, 1, 1));

    //point light
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.pointLightPos, 1, &(_pointLight->getPosition())[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.pointLightColor, 1, &(_pointLight->getColor())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.pointLightPos, 1, &(_pointLight->getPosition())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.pointLightColor, 1, &(_pointLight->getColor())[0]);

    //directional light
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.dirLightDir, 1, &(_dirLight->getDirection())[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.dirLightColor, 1, &(_dirLight->getColor())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.dirLightDir, 1, &(_dirLight->getDirection())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.dirLightColor, 1, &(_dirLight->getColor())[0]);


    //spot light
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.spotLightPos, 1, &(_spotlight->getPosition())[0]);
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.spotLightDir, 1, &(_spotlight->getDirection())[0]);
    glProgramUniform1f(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.spotLightAngle, _spotlight->getAngle());
    glProgramUniform3fv(_lightingShaderProgram->getShaderProgramHandle(), _lightingShaderUniformLocations.spotLightColor, 1, &(_spotlight->getColor())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.spotLightPos, 1, &(_spotlight->getPosition())[0]);
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.spotLightDir, 1, &(_spotlight->getDirection())[0]);
    glProgramUniform1f(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.spotLightAngle, _spotlight->getAngle());
    glProgramUniform3fv(_texShaderProgram->getShaderProgramHandle(), _textureShaderUniformLocations.spotLightColor, 1, &(_spotlight->getColor())[0]);

}

//*************************************************************************************
//
// Engine Cleanup

void A3Engine::_cleanupShaders() {
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    delete _lightingShaderProgram;
}

void A3Engine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();
    glDeleteVertexArrays( 1, &_groundVAO );

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _player;
    delete _arthur;
    delete _clutch;
    delete _saul;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void A3Engine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // use our lighting shader program
    _lightingShaderProgram->useProgram();
    glUniform3fv(_lightingShaderUniformLocations.camPos, 1, &(_arcBall->getPosition())[0]);

    //// BEGIN DRAWING THE GROUND PLANE ////
    // draw the ground plane
    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    _computeAndSendMatrixUniforms(groundModelMtx, viewMtx, projMtx);

    glm::vec3 groundColor(0.3f, 0.8f, 0.2f);
    glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ///


    //// BEGIN DRAWING THE BUILDINGS ////
    for( const BuildingData& currentBuilding : _buildings ) {
        _computeAndSendMatrixUniforms(currentBuilding.modelMatrix, viewMtx, projMtx);

        glUniform3fv(_lightingShaderUniformLocations.materialColor, 1, &currentBuilding.color[0]);

        CSCI441::drawSolidCube(1.0);
    }
    //// END DRAWING THE BUILDINGS ////

    //// BEGIN DRAWING THE PLANE ////
    glm::mat4 modelMtx(1.0f);
    // we are going to cheat and use our look at point to place our plane so that it is always in view
    modelMtx = glm::translate( modelMtx, playerPos);
    // rotate the plane with our camera theta direction (we need to rotate the opposite direction so that we always look at the back)
    //modelMtx = glm::rotate( modelMtx, -_arcBall->getTheta(), CSCI441::Y_AXIS );
    // rotate the plane with our camera phi direction
    modelMtx = glm::rotate( modelMtx,  3.14152f/2.0f, CSCI441::X_AXIS );
    modelMtx = glm::rotate( modelMtx,  (3.14159265358979323846264338327950288f/180.0f) * playerAngle, CSCI441::Z_AXIS );
    // draw our plane now
    _player->drawPlayer(modelMtx, viewMtx, projMtx );
    //// END DRAWING THE PLANE ////

    //// Begin Drawing Characters ////
    glm::mat4 mvpMtx = projMtx * viewMtx;
    glm::mat4 arthurModelMtx = glm::translate(mvpMtx, _arthur->_position);
    arthurModelMtx = glm::scale(mvpMtx, _arthur->_scale);
    glm::mat4 clutchModelMtx = glm::translate(mvpMtx, glm::vec3(3.0f, 0.0f, 0.0f));
    _texShaderProgram->useProgram();
    glUniform3fv(_textureShaderUniformLocations.camPos, 1, &(_arcBall->getPosition())[0]);
    glProgramUniformMatrix4fv(
            _texShaderProgram->getShaderProgramHandle(),
            _textureShaderUniformLocations.mvpMatrix,
            1,
            GL_FALSE, &arthurModelMtx[0][0]);
    _arthur->_model->draw(_texShaderProgram->getShaderProgramHandle());
    glProgramUniformMatrix4fv(
            _texShaderProgram->getShaderProgramHandle(),
            _textureShaderUniformLocations.mvpMatrix,
            1,
            GL_FALSE, &clutchModelMtx[0][0]);
    _clutch->_model->draw(_texShaderProgram->getShaderProgramHandle());

    _saul->draw(projMtx, viewMtx, _texShaderProgram);
    //// End Drawing Characters ////
}

void A3Engine::_updateScene() {
    // fly
    if( _keys[GLFW_KEY_SPACE] ) {
        // go backward if shift held down
        if( _keys[GLFW_KEY_LEFT_SHIFT] || _keys[GLFW_KEY_RIGHT_SHIFT] ) {
            _arcBall->moveBackward(_cameraSpeed.x);
        }
        // go forward
        else {
            _arcBall->moveForward(_cameraSpeed.x);
        }
    }
    // turn right
    if( _keys[GLFW_KEY_D] ) {
        playerAngle += 1;
        float rads = (M_PI / 180.0f) * playerAngle;

        playerDirection = glm::normalize(-glm::vec3(sin(-rads), 0, cos(-rads))) * 0.1f;
    }
    // turn left
    if( _keys[GLFW_KEY_A] ) {
        playerAngle -= 1;
        float rads = (M_PI / 180.0f) * playerAngle;

        playerDirection = glm::normalize(-glm::vec3(sin(-rads), 0, cos(-rads))) * 0.1f;

    }
    // pitch up
    if( _keys[GLFW_KEY_W] ) {
        glm::vec3 nextPos = playerPos + playerDirection;

        if(nextPos[2] > -55 && nextPos[2] < 55 && nextPos[0] > -55 && nextPos[0] < 55){
            _player->moveForward();
            playerPos = playerPos + playerDirection;
            _arcBall->updatePos(playerPos);
            _arcBall->recomputeOrientation();
        }
    }
    // pitch down
    if( _keys[GLFW_KEY_S] ) {
        glm::vec3 nextPos = playerPos - playerDirection;
        if(nextPos[2] > -55 && nextPos[2] < 55 && nextPos[0] > -55 && nextPos[0] < 55){
            _player->moveBackward();
            playerPos = playerPos - playerDirection;
            _arcBall->updatePos(playerPos);
            _arcBall->recomputeOrientation();
        }
    }
}

void A3Engine::run() {
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
        glm::mat4 viewMatrix = _arcBall->getViewMatrix();

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

void A3Engine::_computeAndSendMatrixUniforms(glm::mat4 modelMtx, glm::mat4 viewMtx, glm::mat4 projMtx) const {
    // precompute the Model-View-Projection matrix on the CPU
    glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
    // then send it to the shader on the GPU to apply to every vertex
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.mvpMatrix, mvpMtx);
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.mvpMatrix, mvpMtx);

    // TODO #7: compute and send the normal matrix
    glm::mat3 normalMtx = glm::mat3(glm::transpose(glm::inverse(modelMtx)));
    _lightingShaderProgram->setProgramUniform(_lightingShaderUniformLocations.normMatrix, normalMtx);
    _texShaderProgram->setProgramUniform(_textureShaderUniformLocations.normMatrix, normalMtx);

}

//*************************************************************************************
//
// Callbacks

void lab05_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (A3Engine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab05_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (A3Engine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab05_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (A3Engine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}
