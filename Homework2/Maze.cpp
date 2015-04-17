#include "Maze.h"

const int numVertices = 242, numColors = 242;
const int numRowWalls = 110, numColumnWalls = 110; //11 walls makes 10 rows
const int numCubes = 100;
//const int numIndices = numCubes * 4 * 6;//4 walls per cube * 6 indices per wall

//Default constructor so it can be declared as a global variable before an OpenGL context is created
Maze::Maze(){numCubesToDraw = 0;}

//Constructor for maze initialization once context is available for use by the shader
Maze::Maze(GLuint &vao, GLuint &shader)
{
    numCubesToDraw = 100;
    generateVertices();
    makeGrid();
    fillCubes();
    generateMaze();
    getIndices();
    initVAO(vao, shader);
}

//To watch the maze grow
void Maze::setCubeCount(int cubeCount)
{
    if(cubeCount > 100)
        numCubesToDraw = 100;
    else
        numCubesToDraw = cubeCount;

    getIndices();
}

//This function generates the vertices/points on a 2D plane to be used as points to build our maze walls
//It has two nested loops, the first for the lower level of the grid, and the second for the upper level
//Each set of loops runs 121 times, the total number of vertices set is 242
//The vertices are stored in a one dimension array
void Maze::generateVertices()
{
    //generate points for lower level of grid
    for(int i = 0; i < 11; i++)//i is for the rows
        for( int j = 0; j < 11; j++){//j is for the columns

                //The index is mapped from a "2D array" to our one dimension array
                int index = (i * 11) + j;

                //starting position for the x and z, begin at -1.0
                GLfloat xCoord = (20*((GLfloat)i)/10.0);
                GLfloat zCoord = (20*((GLfloat)j)/10.0);

                //lower level of grid
                vertices[index] = vec4(xCoord, -1.0, zCoord, 1.0);
                colors[index] = vec4(1.0, 1.0, 1.0, 1.0); //white
        }

    //generate points for upper level of grid
    for(int i = 0; i < 11; i++)
        for( int j = 0; j < 11; j++){
                int index = 121 + ((i * 11) + j);
                GLfloat xCoord = (20*((GLfloat)i)/10.0);
                GLfloat zCoord = (20*((GLfloat)j)/10.0);

                //Upper level of grid
                vertices[index] = vec4(xCoord, 1.0, zCoord, 1.0);
                colors[index] = vec4(0.0, 0.0, 0.0, 1.0);//black
        }

}

//This function populates the walls for the rows and the columns of the grid.
//the index goes from 1 to 121. There are 121 row walls and 121 column walls.
//It creates the new walls by passing in an index and orientation
void Maze::makeGrid()
{
    int index = 0;
    for(int i = 1; i <= numVertices/2; i++)
    {
        //We dont want to make columns (vertical walls) if its on the bottom row.
        //The constructor work with rowof(index) - 1 row and would result in array out of bounds
        if(i > 10 ){
            columns[index] = Wall(i, 1); //1 for column wall
            index++;
        }
    }

    index = 0;
    for (int i = 0; i < numVertices/2; i++){
        //We dont want to make row (horizontal walls) if its on the left side of the grid.
        if(i%11!=0){
            rows[index] = Wall(i, 0);//0 for row wall
            index++;
        }
    }
}

//Fill the cubes with walls in two separate loops, one for rows and one for columns
//Row walls for each cube are the current index and current index +10 which is the next row up
//Column walls are for each cube are the current index plus the next one
//We want to skip adding the right edge column wall because it would wrap around to the other side
void Maze::fillCubes()
{
    int index = 0;
    int indicesToSkip = 10;
    for(int i = 0; i < numColumnWalls; i++)
    {
        //Want to skip the right side of the grid which is the last column because
        //it gets added to the next cube which would be on the next row
        if (i == indicesToSkip)   {indicesToSkip+=11; continue;}
        {
            //the cube[index] is populated with the walls[i] and walls[i+1] from the columns
            cubes[index].addWall(columns[i], 1);
            cubes[index].addWall(columns[i+1], 3);
            index++;
        }
    }
    //The row walls for each cube are added with row[i] and the next level row[i+10]
    for (int i = 0; i < numCubes; i++)
    {
        cubes[i].addWall(rows[i], 0);
        cubes[i].addWall(rows[i+10], 2);
    }


}

//This function fills the indices array by first retrieving all the walls associated with each
//cube and then all the indexes associated with each wall
void Maze::getIndices()
{
    indices =  vector<GLuint>();

    int * verts = NULL;//placeholder for array of indices from each wall
    Wall* walls = NULL;//placeholder for array of walls from each cube

    for (int i = 0; i < numCubesToDraw; i++)
    {
        if(cubes[i].isWall())//check if the cube is a wall and should be drawn, or if path and skipped
        {
            walls = cubes[i].getWalls();
            for(int j = 0; j < cubes[i].getNumWalls(); j++)
            {
                verts = walls[j].getIndices();
                for(int k = 0; k < 6; k++)
                {
                    indices.push_back(verts[k]);
                }
            }
        }
    }
}

//This function checks if the camera is within the goal cube
//It checks 2 corners of the cube to get the xMin xMax and zMin zMax
bool Maze::checkIfInGoalCube(vec4 camera)
{
    //index of the two corner vertices in the array of points
    int* goalIndices = goalCube.getBorderIndices();

    //vertices of 2 corners of the goal cube to get full bounds
    vec4 bottomLeft = vertices[goalIndices[0]];
    vec4 topRight = vertices[goalIndices[1]];

    if (camera.x < bottomLeft.x && camera.x > topRight.x
        && camera.z < topRight.z && camera.z > bottomLeft.z)
        {
            return true;
        }
    else
        return false;

}

//Get the number of indices after populating cubes and generating maze
//Not being used because upgraded to vectors from arrays
int Maze::getNumIndices()
{
    int numWalls = 0;
    for(int i = 0; i < numCubes; i++)
    {
        if(cubes[i].isWall())
            numWalls += cubes[i].getNumWalls();
    }

    cout << numWalls * 6;
    return numWalls * 6;
}

/**** COLLISION DETECTION? ****/
//This returns an array of all the vertices on the path through the maze
vec4 * Maze::getPathCoordinates()
{

    vector<vec4> pathIndices = vector<vec4>();

    int * verts = NULL;//placeholder for array of indices from each wall
    Wall* walls = NULL;//placeholder for array of walls from each cube

    for (int i = 0; i < numCubes; i++)
    {
        if(!cubes[i].isWall())//check if the cube is a path, or if wall and skipped
        {
            walls = cubes[i].getWalls();
            for(int j = 0; j < cubes[i].getNumWalls(); j++)
            {
                verts = walls[j].getIndices();
                for(int k = 0; k < 6; k++)
                {
                    pathIndices.push_back(vertices[verts[k]]);
                }
            }
        }
    }
    return pathIndices.data();
}


//Initialize the Vertex Array Object and the Vertex Buffer Object
void Maze::initVAO(GLuint &vao, GLuint &shader)
{
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors );

    // Load shaders and use the resulting shader program
    shader = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( shader );

    // Initialize the vertex position attribute from the vertex shader
    GLuint vPosition = glGetAttribLocation( shader, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    //get the location of the variable in the shader progam
    GLuint vColor = glGetAttribLocation( shader, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vertices)));

}

//Bind the VAO for the maze and call the drawing command.
void Maze::drawMaze(GLuint vao)
{
    glBindVertexArray( vao );

    //call the function to draw the maze. indices.data is an array of indices that tells the
    //drawing order of the indices stored in the gpu
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
}

//To generate the maze we use a 2D array representation of the cubes stored in the 1d array.
//We map from 2d to 1d with the indexFrom2d variable throughout the maze generation.
//This algorithm was based off of the recommended site on the homework assignment
void Maze::generateMaze()
{
    //set first cubes as paths
    int row = 2;
    int col = 0;

    //mapping from 2D coords(row and col) to 1D array of vertices
    int indexFrom2d = (row * 10) + col;

    //Set the cube as a path so we wont include its indices in the drawing
    cubes[indexFrom2d].setAsPath();

    //set second cube as path
    row = 2;
    col = 1;
    indexFrom2d = (row * 10) + col;
    cubes[indexFrom2d].setAsPath();

    //start drawing maze
    recursion(row, col);

}

//recurse through the maze in random directions until no more available cells
void Maze::recursion(int r, int c)
{
     // 4 random directions
     int * randDirs =  new int[4];
     generateRandomDirections(randDirs);

     // Examine each direction
     for (int i = 0; i < 4; i++) {

     int indexFrom2d = (r * 10) + c;

         switch(randDirs[i]){
         case 1:
             //Check whether 2 cells right is out or not
             if (r - 2 <= 0)
                continue;
             //Else set
             else{
                 //move the index two cells to the right
                 indexFrom2d = ((r-2) * 10) + c;
                 if (cubes[indexFrom2d].isWall()) {
                     //If its a wall set it as a path
                     cubes[indexFrom2d].setAsPath();

                     goalCube = cubes[indexFrom2d];//set the goal cube to be the second to last cube to be set as a path

                     //Move the index back one cell
                     indexFrom2d = ((r-1) * 10) + c;
                     cubes[indexFrom2d].setAsPath();

                     //recurse from new cell
                     recursion(r - 2, c);
                 }
             }
             break;
         case 2:
             // Whether 2 cells to the left is out or not
             if (c + 2 >= 9)
                continue;
             else{
                 indexFrom2d = (r * 10) + (c+2);
                 if (cubes[indexFrom2d].isWall()) {
                     cubes[indexFrom2d].setAsPath();

                     goalCube = cubes[indexFrom2d];//set the goal cube to be the second to last cube to be set as a path

                     indexFrom2d = (r * 10) + (c+1);
                     cubes[indexFrom2d].setAsPath();

                     recursion(r, c + 2);
                 }
             }
             break;
         case 3:
             // Whether 2 cells down is out or not
             if (r + 2 >= 9)
                continue;
             else{
                 indexFrom2d = ((r+2) * 10) + c;
                 if (cubes[indexFrom2d].isWall()) {
                     cubes[indexFrom2d].setAsPath();

                     goalCube = cubes[indexFrom2d];//set the goal cube to be the second to last cube to be set as a path

                     indexFrom2d = ((r+1) * 10) + c;
                     cubes[indexFrom2d].setAsPath();

                     recursion(r + 2, c);

                 }
             }
             break;
         case 4:
             // Whether 2 cells to the up is out or not
             if (c - 2 <= 0)
                continue;
             else{
                 indexFrom2d = (r * 10) + (c-2);
                 if (cubes[indexFrom2d].isWall()) {

                     cubes[indexFrom2d].setAsPath();

                     goalCube = cubes[indexFrom2d];//set the goal cube to be the second to last cube to be set as a path

                     indexFrom2d = (r * 10) + (c-1);
                     cubes[indexFrom2d].setAsPath();

                     recursion(r, c - 2);
                 }
             }
             break;
         }
     }
 }

//Generate the random directions for the order the cells should be set as paths
void Maze::generateRandomDirections(int* randoms)
{

    for (int i = 1 ; i < 5; i++)
        randoms[i-1] = i;

    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );

    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = 3; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);

        // Swap arr[i] with the element at random index
        int temp = randoms[i];
        randoms[i] = randoms[j];
        randoms[j] = temp;
    }



}
