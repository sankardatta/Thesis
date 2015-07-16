#include "glCenturai.h"

glCenturai::glCenturai()
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

glCenturai::glCenturai(int width, int height)
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

void glCenturai::clean()
{
   /* glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);*/
}

glCenturai::~glCenturai(void)
{
    clean();
	glfwTerminate();
}

void glCenturai::draw()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f,0.0f);
        glVertex2f(0.5, 0.3);
        glColor3f(0.0f, 1.0f,0.0f);
        glVertex2f(0.3, 0.1);
        glColor3f(0.0f, 0.0f,1.0f);
        glVertex2f(0.75, 0.9);
    glEnd();
    //glfwSwapBuffers(window);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
}

void glCenturai::gameLoop()
{
    //draw();
    while(!glfwWindowShouldClose(window))
    {
        draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}