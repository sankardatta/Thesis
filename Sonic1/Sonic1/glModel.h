#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <malloc.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include <SOIL.h>

using namespace std;
using namespace cv;

class glModel
{
public:
    glModel(void);
    glModel(int, int);
    void opencvHandler(Mat HomographyMatrix, int infLoop);
    ~glModel(void);

public:
    //testing methods
    void glmTest(void);
    void draw(void);
public:
    //GLFWwindow* window;

private:
    void varInit();
    void clean(void);
    GLuint compileShaders(GLuint, string&);
    void loadShaders(const char*, string&);
    void initShaders(const char*, const char*);
    Mat calModelView(Mat H);
    glm::mat4 cameraInverse();
    void cameraPoseFromHomography(Mat& H, Mat& pose);
    
private:
    GLuint vs, fs, program;
    int w, h;
    glm::mat4 cvToGL;
    int texWidth, texHeight;
    GLint colorAttrib;
    unsigned char* image;
    GLint posAttrib, MatrixID;
    GLFWwindow* window;
    //Mat pose;
};

