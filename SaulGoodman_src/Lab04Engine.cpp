#include "Lab04Engine.hpp"

#include <CSCI441/objects.hpp>

#include <fstream>
#include <string>
#include <iostream>
#include "stb_image.h"

//*************************************************************************************
//
// Public Interface

Lab04Engine::Lab04Engine()
         : CSCI441::OpenGLEngine(4, 1, 640, 480, "Lab04: GLSL Shaders") {
    _cam = new ArcballCam();
    _objectIndex = 0;

    for(auto& _key : _keys) _key = GL_FALSE;

    _mousePosition = glm::vec2(MOUSE_UNINITIALIZED, MOUSE_UNINITIALIZED );
    _leftMouseButtonState = GLFW_RELEASE;
}

Lab04Engine::~Lab04Engine() {
    delete _cam;
}

void Lab04Engine::handleKeyEvent(GLint key, GLint action) {
    if(key != GLFW_KEY_UNKNOWN)
        _keys[key] = ((action == GLFW_PRESS) || (action == GLFW_REPEAT));

    if(key == GLFW_KEY_LEFT_SHIFT) {
        _leftShiftState = action;
        if(action == GLFW_RELEASE)
            _cam->zoom(0);
    }

    if(action == GLFW_PRESS) {
        switch( key ) {
            // quit!
            case GLFW_KEY_Q:
            case GLFW_KEY_ESCAPE:
                setWindowShouldClose();
                break;

            //movement
            case GLFW_KEY_W:
                _player->walk(0.1f);
                break;
            case GLFW_KEY_S:
                _player->walk(-0.1f);
                break;
            case GLFW_KEY_A:
                _player->strafe(-0.1f);
                break;
            case GLFW_KEY_D:
                _player->strafe(0.1f);
                break;

            //camera zoom
            case GLFW_KEY_UP:
                _cam->zoom(0.1f);
                break;
            case GLFW_KEY_DOWN:
                _cam->zoom(-0.1f);
                break;

            case GLFW_KEY_1:
            case GLFW_KEY_2:
            case GLFW_KEY_3:
            case GLFW_KEY_4:
            case GLFW_KEY_5:
            case GLFW_KEY_6:
            case GLFW_KEY_7:
                _objectIndex = key - GLFW_KEY_1;
                break;

            default: break; // suppress CLion warning
        }
    }

    if(action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_W:
            case GLFW_KEY_S:
                _player->walk(0);
                break;
            case GLFW_KEY_A:
            case GLFW_KEY_D:
                _player->strafe(0);
                break;
            case GLFW_KEY_UP:
            case GLFW_KEY_DOWN:
                _cam->zoom(0);
                break;
        }
    }
}

void Lab04Engine::handleMouseButtonEvent(GLint button, GLint action) {
    // if the event is for the left mouse button
    if( button == GLFW_MOUSE_BUTTON_LEFT ) {
        // update the left mouse button's state
        _leftMouseButtonState = action;
    }
}

void Lab04Engine::handleCursorPositionEvent(glm::vec2 currMousePosition) {
    // if mouse hasn't moved in the window, prevent camera from flipping out
    if(_mousePosition.x == MOUSE_UNINITIALIZED) {
        _mousePosition = currMousePosition;
    }

    // if the left mouse button is being held down while the mouse is moving
    if(_leftMouseButtonState == GLFW_PRESS) {
        if(_leftShiftState != GLFW_PRESS) {
            // rotate the camera by the distance the mouse moved
            _cam->rotate((currMousePosition.x - _mousePosition.x) * 0.01f,
                         (_mousePosition.y - currMousePosition.y) * 0.01f);
            _player->updateDirection(_cam->getPosition());
        } else {
            _cam->zoom((currMousePosition.y - _mousePosition.y) * 0.01f);
        }
    }

    // update the mouse position
    _mousePosition = currMousePosition;
}

void Lab04Engine::_createGroundBuffers() {
    struct Vertex {
        GLfloat x, y, z;

    };

    Vertex groundQuad[4] = {
            {-1.0f, 0.0f, -1.0f},
            { 1.0f, 0.0f, -1.0f},
            {-1.0f, 0.0f,  1.0f},
            { 1.0f, 0.0f,  1.0f}
    };

    GLushort indices[4] = {0,1,2,3};

    _numGroundPoints = 4;

    glGenVertexArrays(1, &_groundVAO);
    glBindVertexArray(_groundVAO);

    GLuint vbods[2];       // 0 - VBO, 1 - IBO
    glGenBuffers(2, vbods);
    glBindBuffer(GL_ARRAY_BUFFER, vbods[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundQuad), groundQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(_defaultShaderProgram->getAttributeLocation("vPosition"));
    glVertexAttribPointer(_defaultShaderProgram->getAttributeLocation("vPosition"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbods[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

//*************************************************************************************
//
// Engine Setup

void Lab04Engine::_setupGLFW() {
    CSCI441::OpenGLEngine::_setupGLFW();

    // set our callbacks
    glfwSetKeyCallback(_window, lab04_engine_keyboard_callback);
    glfwSetMouseButtonCallback(_window, lab04_engine_mouse_button_callback);
    glfwSetCursorPosCallback(_window, lab04_engine_cursor_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void Lab04Engine::_setupOpenGL() {
    glEnable( GL_DEPTH_TEST );					                    // enable depth testing
    glDepthFunc( GL_LESS );							                // use less than depth test

    glEnable(GL_BLEND);									            // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black
}

void Lab04Engine::_setupShaders() {
    _texShaderProgram = new CSCI441::ShaderProgram("./shaders/customShader.v.glsl",
                                                      "./shaders/customShader.f.glsl");

    _defaultShaderProgram = new CSCI441::ShaderProgram("./shaders/defaultShader.v.glsl",
                                                       "./shaders/defaultShader.f.glsl");
}

// _setupBuffers() /////////////////////////////////////////////////////////////
//
//      Create our VAOs & VBOs. Send vertex data to the GPU for future rendering
//
void Lab04Engine::_setupBuffers() {

    //create player object that contains model/position/etc
    _player = new Player();
    _player->initModel(_texShaderProgram->getAttributeLocation("vPosition"),
                       -1,
                       _texShaderProgram->getAttributeLocation("vTexCoord"));

    //bring in obj models

    _building = new CSCI441::ModelLoader();
    _building->loadModelFile( "models/BreakingBad.obj" );
    _building->setAttributeLocations(_texShaderProgram->getAttributeLocation("vPosition"),
                                  -1,
                                  _texShaderProgram->getAttributeLocation("vTexCoord"));

    _jerma = new CSCI441::ModelLoader();
    _jerma->loadModelFile( "models/WhenTheImposterIsSus.obj" );
    _jerma->setAttributeLocations(_texShaderProgram->getAttributeLocation("vPosition"),
                                     -1,
                                     _texShaderProgram->getAttributeLocation("vTexCoord"));

    //create floor (taken from lab05 :D)
    _createGroundBuffers();
}

void Lab04Engine::_setupTextures() {
    // didn't end up using this
}

void Lab04Engine::_setupScene() {
    _cam->setPosition(glm::vec3(0, 10, 0));
    _cam->setRadius(30);
    _cam->setTheta(0.1 );
    _cam->setPhi(0.1 );
    _cam->_target = _player;
    _cam->recomputeOrientation();

    _player->_position = glm::vec3(50, 0, 50);

    _objectIndex = 2;
}

//*************************************************************************************
//
// Engine Cleanup

void Lab04Engine::_cleanupShaders() {
    // LOOK HERE #4: we're cleaning up our memory again!
    fprintf( stdout, "[INFO]: ...deleting Shaders.\n" );
    glDeleteShader(_texShaderProgram->getShaderProgramHandle());
}

void Lab04Engine::_cleanupBuffers() {
    fprintf( stdout, "[INFO]: ...deleting VAOs....\n" );
    CSCI441::deleteObjectVAOs();

    fprintf( stdout, "[INFO]: ...deleting VBOs....\n" );
    CSCI441::deleteObjectVBOs();

    fprintf( stdout, "[INFO]: ...deleting models..\n" );
    delete _player;
    delete _building;
    delete _jerma;
}

//*************************************************************************************
//
// Rendering / Drawing Functions - this is where the magic happens!

void Lab04Engine::_renderScene(glm::mat4 viewMtx, glm::mat4 projMtx) {
    glm::mat4 mvpMtx = projMtx * viewMtx;

    /// BEGIN DRAW PLAYER ///
    glm::mat4 finalMtx = glm::translate(mvpMtx, _player->_position);
    finalMtx = glm::rotate(finalMtx, _player->_angle, glm::vec3(0,1,0));
    finalMtx = glm::rotate(finalMtx, -glm::pi<GLfloat>()/2, glm::vec3(1,0,0));
    _texShaderProgram->useProgram();

    glProgramUniformMatrix4fv(_texShaderProgram->getShaderProgramHandle(), _texShaderProgram->getUniformLocation("mvpMtx"),
                              1, GL_FALSE, &finalMtx[0][0]);
    _player->_model->draw( _texShaderProgram->getShaderProgramHandle() );


    finalMtx = glm::rotate(finalMtx, _player->_limbAngle, glm::vec3(1,0,0));
    glProgramUniformMatrix4fv(_texShaderProgram->getShaderProgramHandle(), _texShaderProgram->getUniformLocation("mvpMtx"),
                              1, GL_FALSE, &finalMtx[0][0]);
    _player->_limbs->draw( _texShaderProgram->getShaderProgramHandle() );
    /// END DRAW PLAYER ///


    //// BEGIN DRAWING THE GROUND PLANE ////
    glUseProgram(_defaultShaderProgram->getShaderProgramHandle());

    glm::mat4 groundModelMtx = glm::scale( glm::mat4(1.0f), glm::vec3(WORLD_SIZE, 1.0f, WORLD_SIZE));
    glm::mat4 finalGroundMtx = projMtx*viewMtx*groundModelMtx;

    _defaultShaderProgram->setProgramUniform(_defaultShaderProgram->getUniformLocation("mvpMtx"), finalGroundMtx);

    glm::vec3 groundColor(0.3f, 0.8f, 0.2f);
    glUniform3fv(_defaultShaderProgram->getUniformLocation("matColor"), 1, &groundColor[0]);

    glBindVertexArray(_groundVAO);
    glDrawElements(GL_TRIANGLE_STRIP, _numGroundPoints, GL_UNSIGNED_SHORT, (void*)0);
    //// END DRAWING THE GROUND PLANE ////


    /// BEGIN DRAW BUILDING ///
    glm::mat4 buildingModelMtx = glm::scale(glm::mat4(1.0), glm::vec3(0.03, 0.03, 0.03));
    glm::mat4 finalBuildingMtx = projMtx*viewMtx*buildingModelMtx;
    finalBuildingMtx = glm::translate(finalBuildingMtx, glm::vec3(0, 5, 2000));
    _texShaderProgram->useProgram();
    glProgramUniformMatrix4fv(_texShaderProgram->getShaderProgramHandle(), _texShaderProgram->getUniformLocation("mvpMtx"),
                              1, GL_FALSE, &finalBuildingMtx[0][0]);

    _building->draw( _texShaderProgram->getShaderProgramHandle() );
    /// END DRAW BUILDING ///

    /// BEGIN DRAW JERMA ///
    glm::mat4 jermaModelMtx = glm::translate(glm::mat4(1.0), _susPosition + glm::vec3(0,12,-5));
    glm::mat4 finalJermaMtx = projMtx*viewMtx*jermaModelMtx;
    finalJermaMtx = glm::scale(finalJermaMtx, glm::vec3(2, 2, 2));
    glProgramUniformMatrix4fv(_texShaderProgram->getShaderProgramHandle(), _texShaderProgram->getUniformLocation("mvpMtx"),
                              1, GL_FALSE, &finalJermaMtx[0][0]);

    _jerma->draw( _texShaderProgram->getShaderProgramHandle() );
    /// END DRAW JERMA ///


}

void Lab04Engine::_updateScene() {
    _cam->recomputeOrientation();
    _cam->updateZoom();
    _player->updatePosition();

    //confine player to grid
    if(_player->_position.x > WORLD_SIZE) {
        _player->_position.x = WORLD_SIZE;
    }
    if(_player->_position.z > WORLD_SIZE) {
        _player->_position.z = WORLD_SIZE;
    }

    if(_player->_position.x < -WORLD_SIZE) {
        _player->_position.x = -WORLD_SIZE;
    }
    if(_player->_position.z < -WORLD_SIZE) {
        _player->_position.z = -WORLD_SIZE;
    }

    _susPosition += (0.005f * (_player->_position - _susPosition));
}

void Lab04Engine::run() {
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
        // with a FOV of 45 degrees, for our current aspect ratio, and Z ranges from [0.001, 100].
        glm::mat4 projectionMatrix = glm::perspective( 45.0f, (GLfloat) framebufferWidth / (GLfloat) framebufferHeight, 0.001f, 300.0f );

        // set up our look at matrix to position our camera
        glm::mat4 viewMatrix = _cam->getViewMatrix();

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

void Lab04Engine::_readTextFromFile( const char* filename, char* &output ) {
    std::string buf = std::string("");
    std::string line;

    std::ifstream in( filename );
    while( getline(in, line) ) {
        buf += line + "\n";
    }
    output = new char[buf.length()+1];
    strncpy(output, buf.c_str(), buf.length());
    output[buf.length()] = '\0';

    in.close();
}

void Lab04Engine::_printLog( GLuint handle ) {
    int infoLogLength = 0;
    int maxLength;

    // check if the handle is to a vertex/fragment shader
    if( glIsShader( handle ) ) {
        glGetShaderiv(  handle, GL_INFO_LOG_LENGTH, &maxLength );
    }
    // check if the handle is to a shader program
    else {
        glGetProgramiv( handle, GL_INFO_LOG_LENGTH, &maxLength );
    }

    // create a buffer of designated length
    char infoLog[maxLength];

    bool isShader;
    if( glIsShader( handle ) ) {
        // get the info log for the vertex/fragment shader
        glGetShaderInfoLog( handle, maxLength, &infoLogLength, infoLog );

        isShader = true;
    } else {
        // get the info log for the shader program
        glGetProgramInfoLog( handle, maxLength, &infoLogLength, infoLog );

        isShader = false;
    }

    // if the length of the log is greater than 0
    if( infoLogLength > 0 ) {
        // print info to terminal
        printf( "[INFO]: %s Handle %d: %s\n", (isShader ? "Shader" : "Program"), handle, infoLog );
    }
}

GLuint Lab04Engine::_compileShader( const char* filename, GLenum shaderType ) {
    char *shaderString;

    // LOOK HERE #1: read in each text file and store the contents in a string
    _readTextFromFile( filename, shaderString );

    GLuint shaderHandle;

    shaderHandle = glCreateShader(shaderType);

    glShaderSource(shaderHandle, 1, (const char**)&shaderString, NULL);

    _printLog( shaderHandle );

    // we are good programmers so free up the memory used by each buffer
    delete [] shaderString;

    glCompileShader(shaderHandle);

    _printLog( shaderHandle );

    return shaderHandle;
}


//*************************************************************************************
//
// Callbacks

void lab04_engine_keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the key and action through to the engine
    engine->handleKeyEvent(key, action);
}

void lab04_engine_cursor_callback(GLFWwindow *window, double x, double y ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the cursor position through to the engine
    engine->handleCursorPositionEvent(glm::vec2(x, y));
}

void lab04_engine_mouse_button_callback(GLFWwindow *window, int button, int action, int mods ) {
    auto engine = (Lab04Engine*) glfwGetWindowUserPointer(window);

    // pass the mouse button and action through to the engine
    engine->handleMouseButtonEvent(button, action);
}