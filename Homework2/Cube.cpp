#include "Cube.h"

Cube::Cube()
{
    isAWall = true;
}

//set the cube as path so as not to be counted in the drawing
void Cube::setAsPath()
{
    isAWall = false;
}

//returns if wall or not
bool Cube::isWall()
{
    return isAWall;
}

//Add a wall to the cube.
// side 1 is the left column wall
// side 3 is the right column wall
// side 2 is the lower row wall
// side 4 is the upper row wall
void Cube::addWall(Wall wall, int side)
{
    walls[side] = wall;
}

//removes the specified wall from the cube
void Cube::removeWall(int side)
{
    walls[side] = Wall();

}

//Returns the number of walls that are active in this cube
int Cube::getNumWalls()
{
    int numWalls = 0;
    for(int i = 0; i < 4; i++)
    {
        if(!walls[i].isPlaceholder())
        {
            numWalls++;

        }
    }
    return numWalls;
}

//This function returns an array of walls for this cube
Wall* Cube::getWalls()
{
    for(int i = 0; i < 4; i++)
    {
        if(!walls[i].isPlaceholder())
            wallsToUse.push_back(walls[i]);
    }
    return wallsToUse.data();
}

//This returns indices of 2 corners of the cube to get the xMin xMax zMin zMax
//The 2 indices are the bottom of the left column wall vertex and the top of the right column wall
int* Cube::getBorderIndices()
{
    int indices[2];

    indices[0] = walls[1].getIndices()[0];
    indices[1] = walls[3].getIndices()[1];

    return indices;
}
