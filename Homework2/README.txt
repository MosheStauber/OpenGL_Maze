Homework #2

To render the maze I use a nested loop to generate a grid of 11 x 11 points with variating "x" and "z" coordiantes

but a constant "y" value of 0.0 for the lower level. Then I generate the same grid with a "y" value of 1.0

for the upper level. These points/vertices are stored in a one dimensional array.


WALL:
With these points created, Wall objects are assigned vertices specified by an index argument in the constructor.

Each Wall object has a list of 6 indices belonging to the array of points/vertices generated earlier.

The indices are decided based on the index argument, and the orientation of the wall(row or column)

The Wall objects are separated into two arrays, one for row walls and one for column walls of the grid.



CUBE:
Once I have the walls with their indices, I create an array of Cube objects each of which contain a set

of 4 walls(which have their vertices specified already), 2 row walls and 2 column walls.

note:
The positions of the walls and vertices are already in their correct location so all the Cube object really 

does is bind four walls together.



MAZE:
I then generate the maze using a version of the algorithm that was recommended on the assignment document.

This finds a path and removes the cubes that are in the path by calling Cube::setAsPath().


Once I have the maze with the path generated and all the cubes that needed to be removed are gone,

I populate a vector with the indices to the points/vertices array which are contained in each wall 

of every cube. 

The maze object has a method Maze::drawMaze(GLuint vao) which binds a vertex array object 

to the current context and then calls glDrawElements with the data in the indices array.


CAMERA:
The camera is a separate object and the function Camera::getCamera() returns the view matrix to be used 

in the shader. The matrix returned depends on the boolean value if the camera is in birds eye mode.


GOAL REACHED:
To check if the player reached the end of the maze i test the camera's against the boundaries of the goal cube.

The boundaries of the goal cube are obtained by check two vertices in the cube. The bottom-left corner(zMin and 
xMax) and the top-right corner(zMax and xMin)   
 

EXTRA FEATURES:

I added a camera tracker which records and displays the previous locations of the camera if in birds eye view.

Press "t" on the keyboard to toggle the tracker.


You can watch the maze grow by commenting out the marked lines of code in the display function
