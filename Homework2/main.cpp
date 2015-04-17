#include "Maze.h"
#include "Camera.h"
#include <windows.h>

GLuint modelID, viewID, projectionID;  // model view and projection matrix uniform shader variable locations
GLuint mazeVao, cameraVao;
GLuint shader;
GLuint numIndices;

Maze maze;
Camera camera;
bool cameraTracker = false;

// Projection transformation parameters
GLfloat  fovy = 90, aspect = 1.0;
GLfloat  zNear = -0.1, zFar = 1.0;

time_t  startTime, currentTime;

void
init( void )
{
    time(&startTime);

    maze = Maze(mazeVao,shader);
    camera.init(cameraVao, shader);

    //Collision detection not implemented yet
    vec4 * pathIndices = maze.getPathCoordinates();

    viewID = glGetUniformLocation( shader, "view" );
    projectionID = glGetUniformLocation( shader, "projection" );

    //enable hidden surface removal
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0, 0.0, 1.0, 1.0 ); // black background
}

//----------------------------------------------------------------------------

//This function calculates the transformation matrices and displays the data
void
display( void )
{
    time(&currentTime);

    // clear the screen and the depth buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv( viewID, 1, GL_TRUE, camera.getCamera());

    //create matrix for orthographic projection
    mat4  p = Perspective( fovy, aspect, zNear, zFar);

    //send matrix over to shader
    glUniformMatrix4fv( projectionID, 1, GL_TRUE, p);


    /*****************
    uncomment to watch maze grow

    maze.setCubeCount( pow(2,(currentTime - startTime) ));
    ************/


    maze.drawMaze(mazeVao);

    if (camera.isBirdsEye() && cameraTracker)   camera.drawCameraTracker();

    if(maze.checkIfInGoalCube(camera.getPosition()))
    {
        cout <<"You win!";
        exit( EXIT_SUCCESS );

    }

    glutSwapBuffers();

}

//----------------------------------------------------------------------------

void
idle( void )
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------


//This function handles keyboard entry
// If escape is entered the program exits
//If spacebar is entered the camera is in birds eye view
//If t is entered the camera tracker is displayed
void
keyboard( unsigned char key, int x, int y )
{
    switch(key){
    case 27: //escape key
        exit( EXIT_SUCCESS );
        break;
    case ' '://spacebar sets it to a birds eye view
        camera.makeBirdsEye();
        break;
    case 't':
        cameraTracker = !cameraTracker;
    }
    glutPostRedisplay();
}

//this handles the camera rotation when the directional keys are pressed
void specialKeys(int key, int x, int y)
{
    switch (key)
	{
		case GLUT_KEY_LEFT:
            camera.turnLeft();
			break;
		case GLUT_KEY_RIGHT:
            camera.turnRight();
            break;
		case GLUT_KEY_UP:
	       camera.moveForward();
			break;
		case GLUT_KEY_DOWN:
            camera.moveBackward();
			break;
	}
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize( 512, 512 );

    glutCreateWindow( "Homework 2" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutIdleFunc( idle );
    glutKeyboardFunc( keyboard );
    glutSpecialFunc( specialKeys );

    glutMainLoop();
    return 0;
}
