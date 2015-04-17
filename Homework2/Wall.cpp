#include "Wall.h"

Wall::Wall()
{
    placeholder = true;

}

//This function checks orientation and call the appropriate method
Wall::Wall(int index, int orientation)
{
    placeholder = false;

    if(orientation == 0)
    {
        makeRow(index);
    }
    else
    {
        makeColumn(index);
    }

}

//This function returns the array of indices in this wall
int * Wall::getIndices()
{
    return indices;

}

//This function takes an index as an argument and populates the array of indices
//for a row wall based on that index. The row walls are horizontal and the indices specify
//6 vertices that make up two triangles for each wall
void Wall::makeRow(int index)
{
    int indexAbove = index + 121;

    indices[0] = (index);
    indices[1] = (indexAbove);
    indices[2] = (indexAbove - 1);
    indices[3] = (indexAbove - 1);
    indices[4] = (index -1);
    indices[5] = (index);
}

//This function takes an index as an argument and populates an array of indices
//for a column wall based on that index. The column walls are veritcal and the indices specify
//6 vertices that make up two triangles for each wall
void Wall::makeColumn(int index)
{
    int indexAbove = index + 121;

    indices[0] = (index);
    indices[1] = (index - 11);
    indices[2] = (indexAbove - 11);
    indices[3] = (indexAbove - 11);
    indices[4] = (indexAbove);
    indices[5] = (index);
}


