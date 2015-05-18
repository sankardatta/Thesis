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
