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
    std::cout<<"It's over" <<endl;
    std::getchar();
}
void glCenturai::mouseCallback(GLFWwindow* window, int button, int action, int mode)
{
    void * ob = glfwGetWindowUserPointer(window);
    glCenturai * gl = static_cast<glCenturai *>(ob);
    cout << "button:" <<button << " action:"<<action <<" mode:"<<mode <<endl;
    if(action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &gl->initXCur, &gl->initYCur);
    }
}

void glCenturai::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //cout << "key:" <<key <<" scancode:"<<scancode << " action:"<<action <<" mode:"<<mode <<endl;

    void * ob = glfwGetWindowUserPointer(window);
    glCenturai * gl = static_cast<glCenturai *>(ob);
    
    if (action == GLFW_REPEAT)
        gl->speed = gl->speed + 0.2f;
    else
        gl->speed = 1.0f;
    
    GLfloat speed = gl->speed * 0.001f;
    speed = speed > 0.08f ? 0.08:speed;
    cout << "Speed:" <<speed <<endl;

    switch(key)
    {
        case GLFW_KEY_E:
            {
                glTranslatef(0.0f,speed,0.0f);
            break;
            }
        case GLFW_KEY_S:
            {
            glTranslatef(-speed,0.0f,0.0f);
            break;
            }
        case GLFW_KEY_D:
            {
            glTranslatef(0.0f,-speed,0.0f);
            break;
            }
        case GLFW_KEY_F:
            {
            glTranslatef(speed,0.0f,0.0f);
            break;
            }
        case GLFW_KEY_X:
            {
                exit(0);
            }

        default:
            break;
    }
}

void glCenturai::draw()
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

void glCenturai::gameLoop()
{
    //draw();
    speed = 1.0f;
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, *keyCallback );
    glfwSetMouseButtonCallback(window, *mouseCallback);
    while(!glfwWindowShouldClose(window))
    {
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &xCurPos, &yCurPos);
            cout << "x:" << xCurPos << " y:" << yCurPos << endl;
            glRotatef(0.5f * (initXCur - xCurPos), 0.0f,1.0f,0.0f);
            glRotatef(0.5f * (initYCur - yCurPos), 1.0f,0.0f,0.0f);
            initXCur = xCurPos;
            initYCur = yCurPos;
        }
        draw();
        glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}