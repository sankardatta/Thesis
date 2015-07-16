#pragma once
#define GLEW_STATIC
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
#include <SOIL.h>

using namespace std;

class glCenturai
{
public:
    glCenturai(void);
    glCenturai(int, int);
    void draw(void);
    void gameLoop(void);
    ~glCenturai(void);

private:
    void clean(void);

public:
    GLFWwindow* window;
};

