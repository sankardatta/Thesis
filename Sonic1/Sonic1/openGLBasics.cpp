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
#include <SOIL.h>

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
    glViewport(0, 0, 800, 600);
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

void openGLBasics::drawFromElement()
{
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLfloat vertices[] = {
     0.5f,  0.5f,  // Top Right
     0.5f, -0.5f,  // Bottom Right
    -0.5f, -0.5f,  // Bottom Left
    -0.5f,  0.5f   // Top Left 
    };

    GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
    };

    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    initShaders("positionShaderOne.vsh", "colorShaderOne.fsh");
    cout <<"Here in drawFromElements" <<endl;
    GLint posAttrib = glGetAttribLocation(program, "position");
    glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttrib);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(0);
    glfwSwapBuffers(window);
}
void openGLBasics::mainGL()
{
    int width, height;
    unsigned char* image = SOIL_load_image("C:\\Users\\Sankar\\Desktop\\1.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    cout <<"Image Width: " <<width <<endl << "Image height: "<< height<< endl;
    float vertices[] = { 0.0f,  0.5f, // Vertex 1 (X, Y)
                        0.5f, -0.5f, 
                        -0.5f, -0.5f  
                        };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    
    glLoadIdentity();
    glTranslatef(-0.5f, -0.5f, 0.0f);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f,0.0f);
        glVertex2f(0.5, 0.3);
        glColor3f(0.0f, 1.0f,0.0f);
        glVertex2f(0.3, 0.1);
        glColor3f(0.0f, 0.0f,1.0f);
        glVertex2f(0.75, 0.9);
    glEnd();
    
    GLfloat     rtri = 60.0f;
    glTranslatef(0.5f, 0.5f, 0.0f);
    glRotatef(rtri,1.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f,0.0f);
        glVertex2f(0.75, 0.3);
        glColor3f(0.0f, 1.0f,0.0f);
        glVertex2f(0.3, 0.1);
        glColor3f(0.5f, 0.5f,0.0f);
        glVertex2f(0.3, 0.9);
        glColor3f(0.0f, 0.0f,1.0f);
        glVertex2f(0.75, 0.9);
    glEnd();
    glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    glClearColor(0.3f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    rtri = 10.0f;
    //glTranslatef(0.5f, 0.5f, 0.0f);
    glRotatef(rtri,0.0f,0.0f,1.0f);
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);          // Top Of Triangle (Front)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f(-0.5f,-0.5f, 0.5f);          // Left Of Triangle (Front)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f( 0.5f,-0.5f, 0.5f);          // Right Of Triangle (Front)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);          // Top Of Triangle (Right)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f( 0.5f,-0.5f, 0.5f);          // Left Of Triangle (Right)
        glColor3f(0.0f,0.5f,0.0f);          // Green
        glVertex3f( 0.5f,-0.5f, -0.5f);         // Right Of Triangle (Right)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);          // Top Of Triangle (Back)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f( 0.5f,-0.5f, -0.5f);         // Left Of Triangle (Back)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f(-0.5f,-0.5f, -0.5f);         // Right Of Triangle (Back)

        glColor3f(1.0f,0.0f,0.0f);          // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);          // Top Of Triangle (Left)
        glColor3f(0.0f,0.0f,1.0f);          // Blue
        glVertex3f(-0.5f,-0.5f,-0.5f);          // Left Of Triangle (Left)
        glColor3f(0.0f,1.0f,0.0f);          // Green
        glVertex3f(-0.5f,-0.5f, 0.5f);          // Right Of Triangle (Left)
    glEnd();                        // Done Drawing The Pyramid
    glfwSwapBuffers(window);
    std::this_thread::sleep_for(std::chrono::seconds(1));



    GLint posAttrib;
    initShaders("positionShaderOne.vsh", "colorShaderOne.fsh");
    posAttrib = glGetAttribLocation(program, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
    
    
    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );
    glDrawArrays(GL_TRIANGLES, 0, 3); // this will need the shaders
    glfwSwapBuffers(window);    
    std::this_thread::sleep_for(std::chrono::seconds(1));

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
    std::this_thread::sleep_for(std::chrono::seconds(1));

    drawFromElement();

    //Cube
    
    //float cubeVerts[] = { 0.0f,  0.0f, 0.0f,
    //                 1.0f,  0.0f, 0.0f,
    //                 1.0f,  1.0f, 0.0f,
    //                 0.0f,  1.0f, 0.0f,
    //                 1.0f,  0.0f, 1.0f,
    //                 1.0f,  1.0f, 1.0f,
    //                 0.0f,  1.0f, 1.0f,
    //                 0.0f,  1.0f, 0.0f
    //                    };
    //float cubeColor[] = { 0.0f,  0.0f, 0.0f,
    //                    1.0f,  0.0f, 0.0f,
    //                    1.0f,  1.0f, 0.0f,
    //                    0.0f,  1.0f, 0.0f,
    //                    1.0f,  0.0f, 1.0f,
    //                    1.0f,  1.0f, 1.0f,
    //                    0.0f,  1.0f, 1.0f,
    //                    0.0f,  1.0f, 0.0f
    //                    };
    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //GLuint cubeVbo;
    //glGenBuffers(1, &cubeVbo);
    //glBindVertexArray(vao);
    //glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
    //

    //initShaders("cube.vsh", "colorShaderOne.fsh");
    //posAttrib = glGetAttribLocation(program, "position");
    //glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(posAttrib);

    //glClear(GL_COLOR_BUFFER_BIT );
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(3, GL_FLOAT, 0, cubeVerts);
    ////glColorPointer(3, GL_FLOAT, 0, cubeColor);
    //cout <<"Initialized" <<endl;
    ////std::this_thread::sleep_for(std::chrono::seconds(5));
    //glBegin(GL_TRIANGLES);
    //    glArrayElement(3);
    //    glArrayElement(1);
    //    glArrayElement(2);
    //glEnd();
    //glBindVertexArray(0);
    //glfwSwapBuffers(window);
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    
    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);
    //glEnableClientState(GL_VERTEX_ARRAY);
    //GLfloat g_vertex_buffer_data[] = { -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    //                                                -1.0f,-1.0f, 1.0f,
    //                                                -1.0f, 1.0f, 1.0f, // triangle 1 : end
    //                                                 1.0f, 1.0f,-1.0f, // triangle 2 : begin
    //                                                -1.0f,-1.0f,-1.0f,
    //                                                -1.0f, 1.0f,-1.0f, // triangle 2 : end
    //                                                 1.0f,-1.0f, 1.0f,
    //                                                -1.0f,-1.0f,-1.0f,
    //                                                 1.0f,-1.0f,-1.0f,
    //                                                 1.0f, 1.0f,-1.0f,
    //                                                 1.0f,-1.0f,-1.0f,
    //                                                -1.0f,-1.0f,-1.0f,
    //                                                -1.0f,-1.0f,-1.0f,
    //                                                -1.0f, 1.0f, 1.0f,
    //                                                -1.0f, 1.0f,-1.0f,
    //                                                 1.0f,-1.0f, 1.0f,
    //                                                -1.0f,-1.0f, 1.0f,
    //                                                -1.0f,-1.0f,-1.0f,
    //                                                -1.0f, 1.0f, 1.0f,
    //                                                -1.0f,-1.0f, 1.0f,
    //                                                 1.0f,-1.0f, 1.0f,
    //                                                 1.0f, 1.0f, 1.0f,
    //                                                 1.0f,-1.0f,-1.0f,
    //                                                 1.0f, 1.0f,-1.0f,
    //                                                 1.0f,-1.0f,-1.0f,
    //                                                 1.0f, 1.0f, 1.0f,
    //                                                 1.0f,-1.0f, 1.0f,
    //                                                 1.0f, 1.0f, 1.0f,
    //                                                 1.0f, 1.0f,-1.0f,
    //                                                -1.0f, 1.0f,-1.0f,
    //                                                 1.0f, 1.0f, 1.0f,
    //                                                -1.0f, 1.0f,-1.0f,
    //                                                -1.0f, 1.0f, 1.0f,
    //                                                 1.0f, 1.0f, 1.0f,
    //                                                -1.0f, 1.0f, 1.0f,
    //                                                 1.0f,-1.0f, 1.0f
    //                                            };
    //for(int i = 0; i < sizeof(g_vertex_buffer_data)/sizeof(g_vertex_buffer_data[0]); i++)
    //    g_vertex_buffer_data[i] = g_vertex_buffer_data[i] / 2.0f;

    //GLuint cube;
    //glGenBuffers(1, &cube);
    //glBindBuffer(GL_ARRAY_BUFFER, cube);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    //initShaders("cube.vsh", "colorShaderOne.fsh");
    //posAttrib = glGetAttribLocation(program, "position");
    //glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(posAttrib);
    //glBindVertexArray(vao);
    ////glEnable(GL_DEPTH_TEST);
    ////glDepthFunc(GL_LESS);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glDrawArrays(GL_TRIANGLES, 0, 12*3);
    //glfwSwapBuffers(window);
    std::getchar();
    //std::this_thread::sleep_for(std::chrono::seconds(10));
    
}