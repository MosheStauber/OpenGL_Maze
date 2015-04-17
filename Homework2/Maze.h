#ifndef MAZE_H
#define MAZE_H

#include "Cube.h"
#include <time.h>


class Maze
{
    public:
        Maze();
        Maze(GLuint &vao, GLuint &shader);
        int getNumIndices();
        void drawMaze(GLuint vao);
        void setCubeCount(int cubeCount);
        vec4 * getPathCoordinates();
        bool checkIfInGoalCube(vec4 camera);//xMin xMax zMin zMax of goal cube


    private:
        void generateVertices();
        void makeGrid();
        void fillCubes();
        void getIndices();
        void initVAO(GLuint &vao, GLuint &shader);
        void generateMaze();
        void recursion(int r, int c);
        void generateRandomDirections(int* randoms);

        Wall rows[110];
        Wall columns[110];
        Cube cubes[100];
        vec4 vertices[242];
        vec4 colors[242];
        vector<GLuint> indices;
        int numCubesToDraw;

        Cube goalCube;
};

#endif // MAZE_H
