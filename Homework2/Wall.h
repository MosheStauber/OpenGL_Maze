#ifndef WALL_H
#define WALL_H


#include "Angel.h"
#include <vector>

using namespace std;
class Wall
{
    public:

        Wall();
        Wall(int index, int orientation);
        int * getIndices();
        bool isPlaceholder(){return placeholder;};

    private:

        int indices[6];
        void makeRow(int index);
        void makeColumn(int index);
        bool placeholder;

};

#endif // WALL_H
