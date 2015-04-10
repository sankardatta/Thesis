#define GLEW_STATIC
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include "openGLBasics.h"

using namespace std;

openGLBasics::openGLBasics(void)
{
    cout<<"Inside GLBasics."<< endl;
}


openGLBasics::~openGLBasics(void)
{
}

void openGLBasics::loadShaders(const char* fn, string& str)
{
    ifstream inFile(fn);
    if(!inFile.is_open())
    {
        cout << "File " << fn << " could not be opened !!!" <<endl;
        return;
    }
    char tmp[300];
    while(!inFile.eof())
    {
        inFile.getline(tmp, 300);
        str = str + tmp;
        str = str + '\n';
    }
}

GLuint openGLBasics::compileShaders(GLuint mode, string& str)
{
    const GLchar* source= str.c_str();
    GLuint shader = glCreateShader(mode);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    char error[1000];
    glGetShaderInfoLog(shader, 1000, NULL, error);
    cout << "Shader " <<mode << " \n Compile message: " << error <<endl;
    return shader;
}

void openGLBasics::initShaders(const char* vshader, const char* fshader)
{
    string source;
    loadShaders(vshader, source);
    vs = compileShaders(GL_VERTEX_SHADER, source);
    source = "";
    loadShaders(fshader, source);
    fs = compileShaders(GL_FRAGMENT_SHADER, source);

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    //glBindFragDataLocation(program, 0, "outColor");
    glLinkProgram(program);
    glUseProgram(program);
}

void openGLBasics::clean()
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}
void openGLBasics::mainGL()
{
    float vertices[] = { 0.0f,  0.5f, // Vertex 1 (X, Y)
                        0.5f, -0.5f, 
                        -0.5f, -0.5f  
                        };
    //glfw
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);  //GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Fullsreen
    glfwMakeContextCurrent(window);

    //glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
        cout << "glewInit failed, aborting. Code " << err << ". " << endl;
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    
    initShaders("positionShaderOne.vsh", "colorShaderOne.fsh");
    GLint posAttrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    
    
    
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    glfwSwapBuffers(window);

    //while(!glfwWindowShouldClose(window))
    //{
    //      glDrawArrays(GL_TRIANGLES, 0, 3);
    //    //glClear(GL_COLOR_BUFFER_BIT);
    //    //glColor3f(1.0, 0.0, 0.0);
    //    /*glBegin(GL_LINES);
    //        glVertex2f(180.0, 10.0);
    //        glVertex2f(50.0, 150.0);
    //    glEnd();*/
    //    glFlush();
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //    break;
    //}
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    clean();
	glfwTerminate();

}