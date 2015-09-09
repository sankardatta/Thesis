#include "glModel.h"


glModel::glModel(void)
{
    glfwInit();
    window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    cout << "Created Window" <<endl;
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
        cout << "glewInit failed, aborting. Code " << err << ". " << endl;
    glViewport(0, 0, 800, 600);
}

glModel::glModel(int width, int height)
{
    glfwInit();
    window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    cout << "Created Window" <<endl;
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
        cout << "glewInit failed, aborting. Code " << err << ". " << endl;
    glViewport(0, 0, width, height);
}

void glModel::clean()
{
   /* glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);*/
}

glModel::~glModel(void)
{
    //clean();
	glfwTerminate();
}

void glModel::draw()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3f(0.5f, 0.5f,0.0f);
        glVertex2f(0.5, 0.3);
        glColor3f(0.5f, 0.5f,0.0f);
        glVertex2f(-0.5, 0.3);
        glColor3f(0.5f, 0.5f, 0.0f);
        glVertex2f(-0.5, -0.3);
        glColor3f(0.5f, 0.5f, 0.0f);
        glVertex2f(0.5, -0.3);
    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.3f, 0.0f,0.0f);
        glVertex3f(0.1, 0.06, 0.1);
        glColor3f(0.3f, 0.0f,0.0f);
        glVertex3f(-0.1, 0.06, 0.1);
        glColor3f(0.3f, 0.0f, 0.0f);
        glVertex3f(-0.1, -0.06, 0.1);
        glColor3f(0.3f, 0.0f,0.0f);
        glVertex3f(0.1, -0.06, 0.1);
    glEnd();

    //glfwSwapBuffers(window);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}

void glModel::opencvHandler()
{
    while(!glfwWindowShouldClose(window))
    {
        draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}