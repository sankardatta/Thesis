#include <GL/glew.h>
#include <GLFW/glfw3.h>
#pragma once

using namespace std;
class openGLBasics
{
private:
    void loadShaders(const char*, string&);
    GLuint compileShaders(GLuint, string&);
    void drawFromElement(void);
    void drawTexture(void);

public:
    openGLBasics(void);
    void mainGL(void);
    void initShaders(const char*, const char*);
    void clean(void);
    ~openGLBasics(void);

public:
    GLuint vs, fs, program;
    GLFWwindow* window;
};

