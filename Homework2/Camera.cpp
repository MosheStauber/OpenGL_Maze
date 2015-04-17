#include "Camera.h"

Camera::Camera() :  position(4.2,1.0,-1.0,1.0),
                    viewDirection(0.0,0.0,1.0,0.0),
                    up(0.0,1.0,0.0,1.0),
                    birdsEyePos(10.0,12.5,10.0,1.0),
                    birdsEyeLook(10.0,0.0,10.01,1.0),
                    birdsEye(false){ }

//returns a matrix representing the camera using Angel::LookAt function.
//It checks if the camera is in birds eye mode of not and returns the corresponding matrix
mat4 Camera::getCamera()
{
    mat4 eye;
    if(!birdsEye)
        eye = LookAt(position, position + viewDirection, up);
    else
        eye = LookAt(birdsEyePos, birdsEyeLook, up);
    return eye;
}

vec4 Camera::getPosition()
{
    return position;
}

//rotates the viewDirection by -30 degrees (1/3 of a full right turn)
void Camera::turnRight()
{
    viewDirection = RotateY(-30.0) * viewDirection;
}

//rotates the viewDirection by 30 degrees clockwise (1/3 of a full left turn)
void Camera::turnLeft()
{
    viewDirection = RotateY(30.0) * viewDirection;
}

//The viewDirection of the camera is always 1.0 away from the position of the camera.
//To move the camera forward, we add the viewDirection to the current camera position.
//After updating the camera location store the location in the camera tracker array.
void Camera::moveForward()
{
    position += viewDirection;
    tracker.push_back(position);
    trackerColor.push_back(vec4(1.0,0.0,0.0,1.0));
}

//The viewDirection of the camera is always 1.0 away from the position of the camera.
//To move the camera forward, we subtract the viewDirection from the current camera position
void Camera::moveBackward()
{
    position -= viewDirection;
    tracker.push_back(position);
    trackerColor.push_back(vec4(1.0,0.0,0.0,1.0));
}

//switches the birdsEye flag
void Camera::makeBirdsEye()
{
    birdsEye = !birdsEye;
}

//returns the value of the birdsEye fkag
bool Camera::isBirdsEye()
{
    return birdsEye;
}

//Initialize the Vertex Array Object and the Vertex Buffer Object for the camera tracker
void Camera::init(GLuint &vaoIn, GLuint &shader)
{
    tracker.push_back(position);
    trackerColor.push_back(vec4(0.0,1.0,0.0,1.0));

    cVao = vaoIn;
    glGenVertexArrays( 1, &cVao );
    glBindVertexArray( cVao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &cBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, cBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*tracker.size() + sizeof(vec4)*trackerColor.size(), NULL, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*tracker.size(), tracker.data() );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*tracker.size(), sizeof(vec4)*trackerColor.size(), trackerColor.data() );

    // Initialize the vertex position attribute from the vertex shader
    GLuint cPosition = glGetAttribLocation( shader, "vPosition" );
    glEnableVertexAttribArray( cPosition );
    glVertexAttribPointer( cPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    //get the location of the variable in the shader progam
    GLuint cColor = glGetAttribLocation( shader, "vColor" );
    glEnableVertexAttribArray( cColor );
    glVertexAttribPointer( cColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(tracker.data())));
}

//This function updates the VBO containing the trackers location history and draws a point for every location
void Camera::drawCameraTracker()
{
    glBindVertexArray( cVao );
    glBindBuffer( GL_ARRAY_BUFFER, cBuffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*tracker.size() + sizeof(vec4)*trackerColor.size(), NULL, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*tracker.size(), tracker.data() );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*tracker.size(), sizeof(vec4)*trackerColor.size(), trackerColor.data() );

    glLineWidth(3.0);
    glDrawArrays(GL_LINE_STRIP, 0, tracker.size());
}
