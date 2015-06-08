#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <SOIL.h>
#pragma once

using namespace std;
using namespace cv;

class glDraws
{
private:
    void loadShaders(const char*, string&);
    GLuint compileShaders(GLuint, string&);
    void drawFromElement(void);
    void drawBox(vector<double>, vector<double>);
    void drawTexture(void);
    void drawMovingPlane(vector<double>, vector<double>);
    void drawFromCV();
    void drawFromCV(vector<Vec2f>);

public:
    glDraws(void);
    glDraws(int, int);
    Mat mainGL(double rows, double cols);
    Mat mainGL(double rows, double cols, vector<Vec2f> points);
    void mainGL(vector<double>, vector<double>);
    void initShaders(const char*, const char*);
    void clean(void);
    void setValue(vector<Vec2f>);
    ~glDraws(void);

private:
    Mat rotation, viewMatrix;
    GLfloat verticesFromCV[12];
    int texWidth, texHeight;
    unsigned char* image;

public:
    GLuint vs, fs, program;
    GLFWwindow* window;
};

