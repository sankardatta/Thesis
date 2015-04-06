#define GLEW_STATIC
#include <iostream>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "openGLBasics.h"

using namespace std;

openGLBasics::openGLBasics(void)
{
    cout<<"Inside GLBasics."<< endl;
}


openGLBasics::~openGLBasics(void)
{
}

void openGLBasics::mainGL()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window); 
    //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Fullsreen

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
    {
        std::cout << "glewInit failed, aborting. Code " << err << ". " << std::endl;
    }
    
    float vertices[] = { 0.0f,  0.5f, // Vertex 1 (X, Y)
                        0.5f, -0.5f, 
                        -0.5f, -0.5f  
                        };
    GLuint vbo;
    
    try 
    {
        cout << "Generating VBO"<< endl;
        glGenBuffers(1, &vbo);
        cout << "Generated VBO: "<< vbo<< endl;
    }
    catch(exception& e)
    {
        cout<< e.what()<< endl;
    }

    try 
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    }
    catch(exception& e)
    {
        cout<<e.what()<<endl;
    }
    
    /*
    glfwMakeContextCurrent(window);
    
    while(!glfwWindowShouldClose(window))
    {
        glBegin(GL_LINES);
        glVertex2f(10.0, 10.0);
        glVertex2f(50.0, 50.0);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    */
    std::this_thread::sleep_for(std::chrono::seconds(5));
	glfwTerminate();

}