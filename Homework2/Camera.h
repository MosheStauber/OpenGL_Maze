#ifndef CAMERA_H
#define CAMERA_H

#include "Angel.h"
#include <vector>

class Camera
{
    public:
        Camera();
        mat4 getCamera();
        void turnRight();
        void turnLeft();
        void moveForward();
        void moveBackward();
        void makeBirdsEye();
        bool isBirdsEye();
        vec4 getPosition();
        void init(GLuint &cVao, GLuint &shader);
        void drawCameraTracker();

    private:
        vec4 position, viewDirection, up;
        vec4 birdsEyePos, birdsEyeLook;
        bool birdsEye;
        std::vector<vec4> tracker;
        std::vector<vec4> trackerColor;
        GLuint cBuffer;
        GLuint cVao;
        GLuint shader;

};

#endif // CAMERA_H
