#include "camCalib.h"

//CAUTION :: SHITTY CODE DOWN

camCalib::camCalib(void)
{
    cout << "Calibrate camera from images? (y/n): ";
    char c;
    cin >> c;
    if (c == 'y')
        genCamMatrix();//cameraCalib();

    Mat cameraMat;
    vector<double> distCoeffs;
    readCalibFile(cameraMat, distCoeffs);
    solvePNPFromCamera(cameraMat, distCoeffs);
}

void camCalib::solvePNPFromCamera(Mat cameraMat,vector<double> distCoeffs )
{
    CvCapture* capture = cvCaptureFromCAM(0);
    if ( !capture ) 
	{
        cout<< "ERROR: capture is NULL" <<endl;
        getchar();
		exit(0);
    }
    
    // 3D coordinates of chessboard points
    int a = 30; //This is the size of chessboard boxes in mm
    vector<Point3f> objectPoints;
    for(int x=1; x<=8; x++)
    {
        for(int y=1; y<=5; y++) 
            objectPoints.push_back(cv::Point3f(x*a,y*a,0));
    }

    IplImage * frame;
    Mat gray;
    vector<Point2f> imgPoints;
    bool solve;
    vector<double> rvecs, tvecs;
	while ( 1 ) 
     {
        frame = cvQueryFrame( capture );
        if ( !frame ) 
            {
                fprintf( stderr, "ERROR: frame is null...\n" );
                getchar();
                break;
            }
        cvShowImage( "mywindow", frame );
        gray = Mat(frame);
        imgPoints = obtainImagePoints(gray);
        if (imgPoints.size() == objectPoints.size())
        {
            solve = solvePnP(objectPoints, imgPoints, cameraMat, distCoeffs, rvecs, tvecs, false, CV_ITERATIVE);
            //cout << "Rvecs: " << rvecs.at(0) << ". " << rvecs.at(1) << ". " << rvecs.at(2) << endl;
            cout << "Tvecs: " <<tvecs.at(0) << " " << tvecs.at(1) << " " << tvecs.at(2) <<endl;
        }
        
        if ( (cvWaitKey(1) & 255) == 'e' ) 
        {
            break;
        }
    }
}

void camCalib::genCamMatrix()
{
    string path = "C:\\Users\\Sankar\\Desktop\\cameraCalib\\";
    string name;
    vector<Mat> images;
    int numberOfImg = 25;
    try
    {
        for(int i = 0; i<=numberOfImg; i++)
        {
            name = path + "Logitech" + to_string(i) + ".jpg"; 
            images.push_back(imread(name));
        }
        // throw 42; catch (int e)
    }
    catch (exception& e)
    {
        cout << "Error in reading  image at " << path <<". Error: " << e.what() <<endl;
    }

    //initializing cameraMatrix
    Mat initIm = images.at(0);
    Mat cameraMat = cameraCalibInitVal(initIm);

    //Obtaining Size
    double w = initIm.cols;
    double h = initIm.rows;
    Size imageSize(w,h);

    int a = 30; //This is the size of chessboard boxes in mm
    vector<Point3f> objectPoints;
    for(int x=1; x<=8; x++)
    {
        for(int y=1; y<=5; y++) 
            objectPoints.push_back(cv::Point3f(x*a,y*a,0));
    }

    //initializing objectPoints and imagePoints
    vector<vector<Point3f>> arrayObjectPoints;
    vector<Point2f> imagePoints;
    vector<vector<Point2f>> arrayImagePoints;
    try
    {
        for(int i=1; i<=numberOfImg; i++)
        {
            arrayObjectPoints.push_back(objectPoints);
            imagePoints = obtainImagePoints(images.at(i));
            if (objectPoints.size() != imagePoints.size())
            {
                throw i;
            }
            arrayImagePoints.push_back(imagePoints);
        }
    }
    catch(int i)
    {
        cout << "Image Point size and Object point size didn't match for image number: " << i <<endl;
    }

    //Initializing distCoeffs
    vector<double> distCoeffs;
    for(int i=0; i <=8; i++) //distCoeffs can have 8 possible values : Refer OpenCV
    {
        distCoeffs.push_back(0.0);
    }

    vector<vector<double>> rvecs, tvecs;
    int flags = 0;
    TermCriteria criteria=TermCriteria( TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON);
    cout<< "Calling CalibrateCamera" <<endl;
    double cameraCalibratedValue = calibrateCamera(arrayObjectPoints, arrayImagePoints, imageSize, 
                                                cameraMat, distCoeffs, rvecs, tvecs, flags, criteria);

    cout << "CameraCalibValue: " <<cameraCalibratedValue <<endl;
    cout << "Camera Matrix: " << cameraMat << endl;
    //cout << "distCoeffs: " << distCoeffs.data() << endl;
    writeIntoFile(cameraMat, distCoeffs);
    //getchar();
    //solvePNPTry(cameraMat, distCoeffs, rvecs, tvecs);
}

vector<Point2f> camCalib::obtainImagePoints(Mat gray)
{
    
    Size patternsize(8,5); //interior number of corners
    //Mat gray = imread("C:/Users/Sankar/Desktop/matteo.jpg"); //source image
    Mat img = gray;
    cvtColor(gray, gray, CV_BGR2GRAY);
    
    vector<Point2f> corners; //this will be filled by the detected corners

    //CALIB_CB_FAST_CHECK saves a lot of time on images
    //that do not contain any chessboard corners

    bool patternfound = findChessboardCorners(gray, patternsize, corners,
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
            + CALIB_CB_FAST_CHECK);

    if(patternfound)
      cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
        TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    return corners;
}

Mat camCalib::cameraCalibInitVal(Mat im)
{
    
    double w = im.cols;
    double h = im.rows;
    Size imageSize(w,h);

    // 3D coordinates of chessboard points
    int a = 30; //This is the size of chessboard boxes in mm
    vector<Point3f> objectPoints;
    for(int x=1; x<=8; x++){
        for(int y=1; y<=5; y++) 
            objectPoints.push_back(cv::Point3f(x*a,y*a,0));
    }

    //cout<< "object Points: " << objectPoints.size() << endl;
    vector<vector<Point3f>> arrayObjectPoints;
    arrayObjectPoints.push_back(objectPoints);

    double aspectRatio=1;
    vector<Point2f> imagePoints;
    Mat cameraInitMat;
    vector<vector<Point2f>> arrayImagePoints;
    
    imagePoints = cameraMatrixFromChessBoard(im);
    arrayImagePoints.push_back(imagePoints);
    //cout << "Image Points: " << imagePoints.size() <<endl;
    cameraInitMat = initCameraMatrix2D(arrayObjectPoints, arrayImagePoints, imageSize, aspectRatio);
    //cout << "camera Initialization Mat: " <<cameraInitMat <<endl;
    
    return cameraInitMat;
}

vector<Point2f> camCalib::cameraMatrixFromChessBoard(Mat gray)
{
    
    Size patternsize(8,5); //interior number of corners
    //Mat gray = imread("C:/Users/Sankar/Desktop/matteo.jpg"); //source image
    Mat img = gray;
    cvtColor(gray, gray, CV_BGR2GRAY);
    
    vector<Point2f> corners; //this will be filled by the detected corners

    //CALIB_CB_FAST_CHECK saves a lot of time on images
    //that do not contain any chessboard corners

    bool patternfound = findChessboardCorners(gray, patternsize, corners,
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
            + CALIB_CB_FAST_CHECK);

    if(patternfound)
      cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
        TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

    //drawChessboardCorners(img, patternsize, Mat(corners), patternfound);
    //imshow("Result", img);
    //waitKey(1);
    return corners;
}

void camCalib::writeIntoFile(Mat cameraMat, vector<double> distCoeffs)
{
    string filename = "calibInfo.xml";
    FileStorage fs(filename, FileStorage::WRITE);
    
    fs << "cameraMatrix" << cameraMat;
    fs << "distCoeffs" << distCoeffs;

    fs.release();
}

void camCalib::readCalibFile(Mat& cameraMat, vector<double>& distCoeffs)
{
    string filename = "calibInfo.xml";
    FileStorage fs(filename, FileStorage::READ);
    
    fs["cameraMatrix"] >> cameraMat;
    fs["distCoeffs"] >> distCoeffs;

    cout << "camera mat:" << cameraMat << endl;
    cout << "dist coeffs:" << distCoeffs.at(0) << endl;
    fs.release();
}

camCalib::~camCalib(void)
{
}
