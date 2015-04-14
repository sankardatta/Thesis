#define GLEW_STATIC
#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include "openGLBasics.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

openGLBasics::openGLBasics(void)
{
    cout <<"Inside GLBasics." <<endl;
    
    glfwInit();
    window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);  //(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); //Fullsreen
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
        cout << "glewInit failed, aborting. Code " << err << ". " << endl;
}


openGLBasics::~openGLBasics(void)
{
    clean();
	glfwTerminate();
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
    glBindFragDataLocation(program, 0, "outColor");
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
   
    
    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    
    initShaders("positionShaderOne.vsh", "colorShaderOne.fsh");
    GLint posAttrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    
    
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    glClear(GL_COLOR_BUFFER_BIT );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    glBegin(GL_TRIANGLES);
        glVertex2f(0.5, 0.3);
        glVertex2f(0.3, 0.1);
        glVertex2f(0.75, 0.9);
    glEnd();
    glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    glClear(GL_COLOR_BUFFER_BIT );
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

    initShaders("positionShaderTwo.vsh", "colorShaderOne.fsh");
    // Get a handle for our "MVP" uniform.
    // Only at initialisation time.
    posAttrib = glGetAttribLocation(program, "vertexPosition_modelspace");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    GLuint MatrixID = glGetUniformLocation(program, "MVP");
 
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);    
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //Cube
    glClear(GL_COLOR_BUFFER_BIT );
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    float cube[] = { 0.0f,  0.0f, 0.0f,
                     1.0f,  0.0f, 0.0f,
                     1.0f,  1.0f, 0.0f,
                     0.0f,  1.0f, 0.0f,
                     1.0f,  0.0f, 1.0f,
                     1.0f,  1.0f, 1.0f,
                     0.0f,  1.0f, 1.0f,
                     0.0f,  1.0f, 0.0f
                        };
    glVertexPointer(3, GL_FLOAT, 0, cube);
    glBegin(GL_TRIANGLES);
    glArrayElement(0);
    glArrayElement(1);
    glArrayElement(2);
    glEnd();
    glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}