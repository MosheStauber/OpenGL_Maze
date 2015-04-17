#ifndef CUBE_H
#define CUBE_H

#include "Wall.h"
class Cube
{
    public:
        Cube();
        void addWall(Wall wall, int side);
        void removeWall(int side);
        Wall* getWalls();
        int getNumWalls();
        bool isWall();
        void setAsPath();
        int* getBorderIndices();

    private:
        Wall walls[4]; //One for each side of the cube besides for top and bottom
        vector<Wall> wallsToUse;
        bool isAWall;
};

#endif // CUBE_H
