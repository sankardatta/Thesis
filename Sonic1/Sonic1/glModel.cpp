#define GLEW_STATIC
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
    varInit();
}

glModel::glModel(int width, int height)
{
    w = width;
    h = height;
    glfwInit();
    window = glfwCreateWindow(width, height, "OpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    cout << "Created Window" <<endl;
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    
    if(err!=GLEW_OK)
        cout << "glewInit failed, aborting. Code " << err << ". " << endl;
    glViewport(0, 0, width, height);
    varInit();
}

GLuint glModel::compileShaders(GLuint mode, string& str)
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

void glModel::loadShaders(const char* fn, string& str)
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

void glModel::initShaders(const char* vshader, const char* fshader)
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

void glModel::clean()
{
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

glModel::~glModel(void)
{
    clean();
	glfwTerminate();
}

void glModel::cameraPoseFromHomography(Mat& H, Mat& pose)
{
    pose = Mat::eye(3, 4, CV_32FC1);      // 3x4 matrix, the camera pose
    float norm1 = (float)norm(H.col(0));
    float norm2 = (float)norm(H.col(1));  
    float tnorm = (norm1 + norm2) / 2.0f; // Normalization value

    //Mat p1 = H.col(0);       // Pointer to first column of H
    //Mat p2 = pose.col(0);    // Pointer to first column of pose (empty)

    //cv::normalize(p1, p2);   // Normalize the rotation, and copies the column to pose
    cv::normalize(H.col(0), pose.col(0));
    //pose.col(0) = p2.col(0);
    //cout << "p2:" << p2 <<endl << "Pose:" << pose;
    //p1 = H.col(1);           // Pointer to second column of H
    //p2 = pose.col(1);        // Pointer to second column of pose (empty)

    //cv::normalize(p1, p2);   // Normalize the rotation and copies the column to pose
    cv::normalize(H.col(1), pose.col(1));
    Mat p1 = pose.col(0);
    Mat p2 = pose.col(1);

    Mat p3 = p1.cross(p2);   // Computes the cross-product of p1 and p2
    Mat c2 = pose.col(2);    // Pointer to third column of pose
    p3.copyTo(c2);       // Third column is the crossproduct of columns one and two

    pose.col(3) = H.col(2) / tnorm;  //vector t [R|t] is the last column of pose
    cout<< "Pose:" << pose << endl;
}

void glModel::glmTest()
{
    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(0.5f, 0.25f, 0.1f));
    //trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.01f));
    //trans = glm::scale(trans, glm::vec3(0.6f, 0.7f, 0.8f));
    trans = glm::rotate(trans, glm::radians(30.0f), glm::vec3(0.0, 0.0, -1.0));
    //trans = glm::rotate(trans, glm::radians(30.0f), glm::vec3(1.0, 0.0, 0.0));
    cout << "trans: " << endl<< glm::to_string(trans) <<endl;

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    
    GLfloat vertices[] = {
     0.16f,  0.233f, 1.0f, 0.0f, 0.0f,  // Top Right
    -0.16f,  0.233f, 1.0f, 1.0f, 1.0f,  // Top Left
    -0.16f, -0.233f, 1.0f, 1.0f, 1.0f, // Bottom Left
     0.16f, -0.233f, 1.0f, 1.0f, 1.0f  // Bottom Right
    };
    
    GLuint indices[] = {  // Note that we start from 0!
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
    };

    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint VBO;
    glGenBuffers(1, &VBO);

    do
    {
        //draw();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttrib);

        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &trans[0][0]);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(0);

        //glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while(!glfwWindowShouldClose(window));
    
}

void glModel::draw()
{
    // this function draws a plane for testing purpose

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT );
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_QUADS);
            glColor3f(0.5f, 0.5f,0.0f);
            glVertex2f(0.16, 0.233);
            glColor3f(0.5f, 0.5f,0.0f);
            glVertex2f(0.16, -0.233);
            glColor3f(0.5f, 0.5f, 0.0f);
            glVertex2f(-0.16, -0.233);
            glColor3f(0.5f, 0.5f, 0.0f);
            glVertex2f(-0.16, 0.233);
        glEnd();

        //glBegin(GL_QUADS);
        //    glColor3f(0.3f, 0.0f,0.0f);
        //    glVertex3f(0.1, 0.06, 0.1);
        //    glColor3f(0.3f, 0.0f,0.0f);
        //    glVertex3f(-0.1, 0.06, 0.1);
        //    glColor3f(0.3f, 0.0f, 0.0f);
        //    glVertex3f(-0.1, -0.06, 0.1);
        //    glColor3f(0.3f, 0.0f,0.0f);
        //    glVertex3f(0.1, -0.06, 0.1);
        //glEnd();

        //glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void glModel::varInit()
{
    
    cvToGL[1][1] = -1;
    cvToGL[2][2] = -1;
    cout<< "cvToGL: "<< glm::to_string(cvToGL)<< endl;
    //image = SOIL_load_image("C:\\Users\\Sankar\\Desktop\\original.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    initShaders("positionShaderOne.vsh", "colorShaderOne.fsh");
    posAttrib = glGetAttribLocation(program, "position");
    colorAttrib = glGetAttribLocation(program, "inColor");
    MatrixID = glGetUniformLocation(program, "MVP");
}

glm::mat4 glModel::cameraInverse()
{
    glm::mat4 intrinsicsInv;

    double f_x = 623.7274584149; // Focal length in x axis
    double f_y = 1.0001525492*f_x; // Focal length in y axis (usually the same?)
    double c_x = 341.0843632075; // Camera primary point x
    double c_y = 232.7543939883; // Camera primary point y

    //Mat inverseCameraMatrix = (Mat_<double>(3, 3) << 1/f_x , 0 , -c_x/f_x , 0 , 1/f_y , -c_y/f_y, 0 , 0 , 1);
    intrinsicsInv[0][0] = 1/f_x;   intrinsicsInv[1][0] = 0;      intrinsicsInv[2][0] = -c_x/f_x;

    intrinsicsInv[0][1] = 0;       intrinsicsInv[1][1] = 1/f_y;  intrinsicsInv[2][1] = -c_y/f_y;

    intrinsicsInv[0][2] = 0;       intrinsicsInv[1][2] = 0;      intrinsicsInv[2][2] = 1;
    
    return intrinsicsInv;
}

Mat glModel::calModelView(Mat H)
{
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();

    // Camera parameters
    double f_x = 623.7274584149; // Focal length in x axis
    double f_y = 1.0001525492*f_x; // Focal length in y axis (usually the same?)
    double c_x = 341.0843632075; // Camera primary point x
    double c_y = 232.7543939883; // Camera primary point y

    double fovY = 1/(f_x/h * 2);
    double aspectRatio = w/h * f_y/f_x;
    double near = .1;  // Near clipping distance
    double far = 1000;  // Far clipping distance
    double frustum_height = near * fovY;
    double frustum_width = frustum_height * aspectRatio;

    double offset_x = (w/2 - c_x)/w * frustum_width * 2;
    double offset_y = (h/2 - c_y)/h * frustum_height * 2;
    // Build and apply the projection matrix
    //glFrustum(-frustum_width - offset_x, frustum_width - offset_x, -frustum_height - offset_y, frustum_height - offset_y, near, far);

    Mat inverseCameraMatrix = (Mat_<double>(3, 3) << 1/f_x , 0 , -c_x/f_x , 0 , 1/f_y , -c_y/f_y, 0 , 0 , 1);
    cout << inverseCameraMatrix;

    // Column vectors of homography
    Mat h1 = (Mat_<double>(3,1) << H.at<double>(0,0) , H.at<double>(1,0) , H.at<double>(2,0));
    Mat h2 = (Mat_<double>(3,1) << H.at<double>(0,1) , H.at<double>(1,1) , H.at<double>(2,1));
    Mat h3 = (Mat_<double>(3,1) << H.at<double>(0,2) , H.at<double>(1,2) , H.at<double>(2,2));

    Mat inverseH1 = inverseCameraMatrix * h1;
    // Calculate a length, for normalizing
    double lambda = sqrt(h1.at<double>(0,0)*h1.at<double>(0,0) + h1.at<double>(1,0)*h1.at<double>(1,0) + h1.at<double>(2,0)*h1.at<double>(2,0));

    Mat rotationMatrix; 
    Mat modelviewMatrix;

    if(!lambda==0)
    {
        lambda = 1/lambda;
        // Normalize inverseCameraMatrix
        inverseCameraMatrix.at<double>(0,0) *= lambda;
        inverseCameraMatrix.at<double>(1,0) *= lambda;
        inverseCameraMatrix.at<double>(2,0) *= lambda;
        inverseCameraMatrix.at<double>(0,1) *= lambda;
        inverseCameraMatrix.at<double>(1,1) *= lambda;
        inverseCameraMatrix.at<double>(2,1) *= lambda;
        inverseCameraMatrix.at<double>(0,2) *= lambda;
        inverseCameraMatrix.at<double>(1,2) *= lambda;
        inverseCameraMatrix.at<double>(2,2) *= lambda;

        // Column vectors of rotation matrix
        Mat r1 = inverseCameraMatrix * h1;
        Mat r2 = inverseCameraMatrix * h2;
        Mat r3 = r1.cross(r2);    // Orthogonal to r1 and r2
  
        // Put rotation columns into rotation matrix... with some unexplained sign changes
        rotationMatrix = (Mat_<double>(3,3) <<  r1.at<double>(0,0) , -r2.at<double>(0,0) , -r3.at<double>(0,0) ,
                                               -r1.at<double>(1,0) , r2.at<double>(1,0) , r3.at<double>(1,0) ,
                                               -r1.at<double>(2,0) , r2.at<double>(2,0) , r3.at<double>(2,0));
 
        // Translation vector T
        Mat translationVector = inverseCameraMatrix * h3;
        translationVector.at<double>(0,0) *= 1;
        translationVector.at<double>(1,0) *= -1;
        translationVector.at<double>(2,0) *= -1;
 
        //cout << endl << endl << "Rotation: " << endl << rotationMatrix << endl;
        //cout << endl << endl << "Translation: " << endl << translationVector << endl;

        SVD decomposed(rotationMatrix); // I don't really know what this does. But it works.
        rotationMatrix = decomposed.u * decomposed.vt;

        modelviewMatrix = (Mat_<double>(4,4) << rotationMatrix.at<double>(0,0), rotationMatrix.at<double>(0,1), rotationMatrix.at<double>(0,2), translationVector.at<double>(0,0),
                                                    rotationMatrix.at<double>(1,0), rotationMatrix.at<double>(1,1), rotationMatrix.at<double>(1,2), translationVector.at<double>(1,0),
                                                    rotationMatrix.at<double>(2,0), rotationMatrix.at<double>(2,1), rotationMatrix.at<double>(2,2), translationVector.at<double>(2,0),
                                                    0,0,0,1);
    }
    else
    {
        cout << "lambda is zero" <<endl;
        modelviewMatrix = (Mat_<double>(4,4) << 1.0, 0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0); 
    }
    return modelviewMatrix;
}

void glModel::opencvHandler(Mat HomographyMatrix, int infLoop)
{
    glm::mat4 MVP;
    //glm::vec3 cx, cy, cz;
    
    //glm::tvec3<double> cx, cy, cz;
    //cx.x = HomographyMatrix.at<double>(0,0);
    //cx.y = HomographyMatrix.at<double>(1,0);
    //cx.z = HomographyMatrix.at<double>(2,0);
    //cout<< "cX:"<< glm::to_string(cx)<< endl;
    //cy.x = HomographyMatrix.at<double>(0,1);
    //cy.y = HomographyMatrix.at<double>(1,1);
    //cy.z = HomographyMatrix.at<double>(2,1);
    //cout<< "cY:"<< glm::to_string(cy)<< endl;
    //cz = glm::cross(cx, cy);
    //cout<< "cZ:"<< glm::to_string(cz)<< endl;
    //
    ////cout  << cz.x << cz.y << cz.z << endl;

    ////first approach
    ////MVP[0][0] = HomographyMatrix.at<double>(0,0); MVP[1][0] = HomographyMatrix.at<double>(0,1); MVP[2][0] = HomographyMatrix.at<double>(0,2); MVP[3][0] = 0.0;

    ////MVP[0][1] = HomographyMatrix.at<double>(1,0); MVP[1][1] = HomographyMatrix.at<double>(1,1); MVP[2][1] = HomographyMatrix.at<double>(1,2); MVP[3][1] = 0.0;

    ////MVP[0][2] = cz.x;                             MVP[1][2] = cz.y;                             MVP[2][2] = cz.z;                             MVP[3][2] = 0;

    ////MVP[0][3] = HomographyMatrix.at<double>(2,0); MVP[1][3] = HomographyMatrix.at<double>(2,2); MVP[2][3] = -HomographyMatrix.at<double>(2,1); MVP[3][3] = 1;


    ////Second Approach is the Best guess
    //MVP[0][0] = HomographyMatrix.at<double>(0,0);   MVP[1][0] = -HomographyMatrix.at<double>(0,1);  MVP[2][0] = -cz.x;  MVP[3][0] =  2 * (HomographyMatrix.at<double>(0,2) - w/2) / w;

    //MVP[0][1] = -HomographyMatrix.at<double>(1,0);  MVP[1][1] = HomographyMatrix.at<double>(1,1);  MVP[2][1] = cz.y;   MVP[3][1] =  -2 * (HomographyMatrix.at<double>(1,2) - h/2) / h;

    //MVP[0][2] = -HomographyMatrix.at<double>(2,0);  MVP[1][2] = HomographyMatrix.at<double>(2,1);  MVP[2][2] = cz.z;   MVP[3][2] =   0 * (-HomographyMatrix.at<double>(2,2));

    //MVP[0][3] = 0;                                  MVP[1][3] = 0;                                 MVP[2][3] = 0;      MVP[3][3] = 1;

    //cout << "MVP:" <<endl << glm::to_string(MVP) <<endl;

    //MVP = cvToGL * MVP;

    //third approach
    //MVP[0][0] = HomographyMatrix.at<double>(0,0);
    //MVP[1][0] = cz.x;
    //MVP[2][0] = HomographyMatrix.at<double>(0,1);
    //MVP[3][0] = HomographyMatrix.at<double>(0,2);

    //MVP[0][1] = HomographyMatrix.at<double>(1,0);
    //MVP[1][1] = cz.y;
    //MVP[2][1] = HomographyMatrix.at<double>(1,1);
    //MVP[3][1] = HomographyMatrix.at<double>(2,2);

    //MVP[0][2] = HomographyMatrix.at<double>(2,0);
    //MVP[1][2] = cz.z;
    //MVP[2][2] = HomographyMatrix.at<double>(2,1);
    //MVP[3][2] = -HomographyMatrix.at<double>(1,2);

    //MVP[0][3] = 0;
    //MVP[1][3] = 0;
    //MVP[2][3] = 0;
    //MVP[3][3] = 1;

    //Fourth Approach
    //Mat pose;
    //cameraPoseFromHomography(HomographyMatrix, pose);
    //
    //MVP[0][0] = pose.at<float>(0,0); MVP[1][0] = pose.at<float>(0,1); MVP[2][0] = pose.at<float>(0,2); MVP[3][0] = pose.at<float>(0,3);

    //MVP[0][1] = -pose.at<float>(1,0); MVP[1][1] = -pose.at<float>(1,1); MVP[2][1] = -pose.at<float>(1,2); MVP[3][1] = pose.at<float>(1,3);

    //MVP[0][2] = -pose.at<float>(2,0); MVP[1][2] = -pose.at<float>(2,1); MVP[2][2] = -pose.at<float>(2,2); MVP[3][2] = pose.at<float>(2,3);

    //MVP[0][3] = 0;                   MVP[1][3] = 0;                   MVP[2][3] = 0;                   MVP[3][3] = 1;//pose.at<float>(3,3);

    //cout << "MVP"<<glm::to_string(MVP)<<endl;

    //MVP = glm::translate(MVP, glm::vec3(1.0f, 1.0f, 0.0f));
    //std::cout << vec.x << vec.y << vec.z << std::endl;

    //Fifth Approach
    Mat modelView = calModelView(HomographyMatrix);
    //cout << endl << endl << "modelView: " << endl << modelView << endl;

    MVP[0][0] = modelView.at<double>(0,0);  MVP[1][0] = modelView.at<double>(0,1);  MVP[2][0] = modelView.at<double>(0,2);  MVP[3][0] =  modelView.at<double>(0,3);

    MVP[0][1] = modelView.at<double>(1,0);  MVP[1][1] = modelView.at<double>(1,1);  MVP[2][1] = modelView.at<double>(1,2);  MVP[3][1] =  modelView.at<double>(1,3);

    MVP[0][2] = modelView.at<double>(2,0);  MVP[1][2] = modelView.at<double>(2,1);  MVP[2][2] = modelView.at<double>(2,2);  MVP[3][2] = 0 * modelView.at<double>(2,3);

    MVP[0][3] = modelView.at<double>(3,0);  MVP[1][3] = modelView.at<double>(3,1);  MVP[2][3] = modelView.at<double>(3,2);  MVP[3][3] =  modelView.at<double>(3,3);

    cout << "MVP:" <<endl << glm::to_string(MVP) <<endl;

    //Mine Fifth approach
/*    double lambda = sqrt(HomographyMatrix.at<double>(0,0)*HomographyMatrix.at<double>(0,0) + HomographyMatrix.at<double>(1,0)*HomographyMatrix.at<double>(1,0) + HomographyMatrix.at<double>(2,0)*HomographyMatrix.at<double>(2,0));
    glm::mat4 inv = cameraInverse();
    glm::mat4 homography =*/ 
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    GLuint VAO2;
    glGenVertexArrays(1, &VAO2);
    
    GLuint EBO;
    glGenBuffers(1, &EBO);
    GLuint EBO2;
    glGenBuffers(1, &EBO2);
    
    GLuint VBO;
    glGenBuffers(1, &VBO);
    GLuint VBO2;
    glGenBuffers(1, &VBO2);

     GLfloat vertices[] = {
     0.16f,  0.233f, 0.0f, 1.0f, 0.0f, 0.0f,  // Top Right
    -0.16f,  0.233f, 0.0f, 1.0f, 1.0f, 1.0f,  // Top Left
    -0.16f, -0.233f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom Left
     0.16f, -0.233f, 0.0f, 1.0f, 1.0f, 1.0f  // Bottom Right
    };
    
     GLfloat lines[] = {
     0.16f,  0.233f, 0.0f, 1.0f, 0.0f, 0.0f,  // Top Right
    -0.16f,  0.233f, 0.0f, 1.0f, 1.0f, 1.0f,  // Top Left
    -0.16f, -0.233f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom Left
     0.16f, -0.233f, 0.0f, 1.0f, 1.0f, 1.0f,  // Bottom Right

     0.16f,  0.233f, 0.5f, 1.0f, 0.0f, 0.0f,  // Top Right
    -0.16f,  0.233f, 0.5f, 1.0f, 1.0f, 1.0f,  // Top Left
    -0.16f, -0.233f, 0.5f, 1.0f, 1.0f, 1.0f, // Bottom Left
     0.16f, -0.233f, 0.5f, 1.0f, 1.0f, 1.0f  // Bottom Right
    };

    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3, 4};  // First Triangle
    //1, 2, 3    // Second Triangle
    //};
    GLuint lineIndices[] = {  // Note that we start from 0!
    0, 1, 2, 3,  // First Quad
    4, 5, 6, 7,
    0, 4, 7, 3,
    0, 1, 5, 4,
    1, 5, 6, 2,
    3, 2, 6, 7
    // Second Triangle
    };

    do
    {
        //draw();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttrib);

        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glBindVertexArray(0);

        glBindVertexArray(VAO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);

        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(posAttrib);

        glEnableVertexAttribArray(colorAttrib);
        glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glBindVertexArray(0);

        //glBindVertexArray(VAO);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
        //glClear(GL_COLOR_BUFFER_BIT);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ////glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        //glBindVertexArray(0);
        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(0);

        glBindVertexArray(VAO2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //GL_LINE
        //glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(0);

        //glfwPollEvents();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }while((!glfwWindowShouldClose(window)) && infLoop);
}
