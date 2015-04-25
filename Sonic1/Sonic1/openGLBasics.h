#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#pragma once

using namespace std;
using namespace cv;

class openGLBasics
{
private:
    void loadShaders(const char*, string&);
    GLuint compileShaders(GLuint, string&);
    void drawFromElement(void);
    void drawTexture(void);

public:
    openGLBasics(void);
    Mat mainGL(double rows, double cols);
    void initShaders(const char*, const char*);
    void clean(void);
    ~openGLBasics(void);

public:
    GLuint vs, fs, program;
    GLFWwindow* window;
};

