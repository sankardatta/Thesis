#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include "imReadBasics.h"
#include "utils.h"
#include "geometry.h"
#include "glDraws.h"

using namespace std;
using namespace cv;

vector<Point2f> cameraMatrix(Mat);
Mat cameraCalibInitVal(Mat);
vector<Point2f> obtainImagePoints(Mat);

void cameraCalibTakeImages()
{
	CvCapture* capture = cvCaptureFromCAM(0);
	if ( !capture ) 
	{
        cout<< "ERROR: capture is NULL" <<endl;
        getchar();
		getchar();
		exit(0);
    }
    IplImage* img;
    IplImage* frame;
	cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    
    int count = 0;
    string path = "C:\\Users\\Sankar\\Desktop\\cameraCalib\\";
    string loc;
    Mat gray, drawIm;
    CvSize size;
    Size patternsize(8,5);
    vector<Point2f> corners;
    bool patternfound;
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
        //cameraMatrix(im);
        if ( (cvWaitKey(3) & 255) == 's' ) {
            size = cvGetSize(frame);
            img = cvCreateImage(size, IPL_DEPTH_16S, 1);
            img = frame;

            loc = path + "Logitech" + to_string(count) + ".jpg";
            count = count + 1;
            cvSaveImage(loc.c_str(), img);
                                            }
		if ( (cvWaitKey(3) & 255) == 'e' ) 
			break;
        
        //Working with chessBoard
        drawIm = gray;
        cvtColor(gray, gray, CV_BGR2GRAY);
    
         //this will be filled by the detected corners

        //CALIB_CB_FAST_CHECK saves a lot of time on images
        //that do not contain any chessboard corners

        patternfound = findChessboardCorners(gray, patternsize, corners,
                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                + CALIB_CB_FAST_CHECK);

        if(patternfound)
          cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
            TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

        drawChessboardCorners(drawIm, patternsize, Mat(corners), patternfound);
        imshow("Result", drawIm);
        //waitKey(0);
    }
	cvReleaseCapture( &capture );
    cvDestroyWindow( "mywindow" );

}

void cameraCalib()
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
        cout << "Error in reading " << numberOfImg << " images from Desktop\\cameraCalib: " << e.what() <<endl;
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
    for(int i=0; i <=8; i++)
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
    getchar();
}

vector<Point2f> obtainImagePoints(Mat gray)
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

Mat cameraCalibInitVal(Mat im)
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
    
    imagePoints = cameraMatrix(im);
    arrayImagePoints.push_back(imagePoints);
    //cout << "Image Points: " << imagePoints.size() <<endl;
    cameraInitMat = initCameraMatrix2D(arrayObjectPoints, arrayImagePoints, imageSize, aspectRatio);
    //cout << "camera Initialization Mat: " <<cameraInitMat <<endl;
    
    return cameraInitMat;
}

void cameraMatInit()
{
    CvCapture* capture = cvCaptureFromCAM(0);
    if ( !capture ) 
	{
        cout<< "ERROR: capture is NULL" <<endl;
        getchar();
		getchar();
		exit(0);
    }

    double w = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	double h = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
    Size imageSize(w,h);

    // 3D coordinates of chessboard points
    int a = 30;
    vector<Point3f> objectPoints;
    for(int x=1; x<=8; x++){
        for(int y=1; y<=5; y++) 
            objectPoints.push_back(cv::Point3f(x*a,y*a,0));
    }

    cout<< "object Points: " << objectPoints.size() << endl;
    vector<vector<Point3f>> arrayObjectPoints;
    arrayObjectPoints.push_back(objectPoints);

    double aspectRatio=1;
    IplImage * frame;
    Mat im;
    vector<Point2f> imagePoints;
    Mat cameraInitMat;
    while ( 1 ) 
    {
        frame = cvQueryFrame( capture );
        im = Mat(frame);
        imagePoints = cameraMatrix(im);

        if ( (cvWaitKey(1) & 255) == 's' ) 
        {
            vector<vector<Point2f>> arrayImagePoints;
            arrayImagePoints.push_back(imagePoints);
            cout << "Image Points: " << imagePoints.size() <<endl;
            cvSaveImage("C:\\Users\\Sankar\\Desktop\\matteo.jpg", frame);
            cameraInitMat = initCameraMatrix2D(arrayObjectPoints, arrayImagePoints, imageSize, aspectRatio);
            cout << "camera Initialization Mat: " <<cameraInitMat <<endl;
        }

        if ( (cvWaitKey(1) & 255) == 'e' ) 
        {
            break;
        }
    }
}

vector<Point2f> cameraMatrix(Mat gray)
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

    drawChessboardCorners(img, patternsize, Mat(corners), patternfound);
    //imshow("Result", img);
    //waitKey(1);
    return corners;
}

void cameraMatrix()
{
    
    Size patternsize(8,5); //interior number of corners
    Mat gray = imread("C:/Users/Sankar/Desktop/matteo.jpg"); //source image
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

    drawChessboardCorners(img, patternsize, Mat(corners), patternfound);
    imshow("Result", img);
    waitKey(0);
}


void takeImage()
{
	CvCapture* capture = cvCaptureFromCAM(0);
	if ( !capture ) 
	{
        cout<< "ERROR: capture is NULL" <<endl;
        getchar();
		getchar();
		exit(0);
    }

	cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
	double w = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	double h = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	cout<<"Width: " <<w<<"\nHeight: "<< h << endl;
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1280);
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 960);

	while ( 1 ) {
        // Get one frame
        IplImage* frame = cvQueryFrame( capture );
        if ( !frame ) {
            fprintf( stderr, "ERROR: frame is null...\n" );
            getchar();
            break;
        }
        cvShowImage( "mywindow", frame );
        // Do not release the frame!

        if ( (cvWaitKey(10) & 255) == 's' ) {
            CvSize size = cvGetSize(frame);
            IplImage* img= cvCreateImage(size, IPL_DEPTH_16S, 1);
            img = frame;
             cvSaveImage("C:\\Users\\Sankar\\Desktop\\matteo.jpg", img);
                                            }
		if ( (cvWaitKey(10) & 255) == 'e' ) 
			break;
    }
	cvReleaseCapture( &capture );
    cvDestroyWindow( "mywindow" );

}


void mainT()
{
	string path = "C:/Users/Sankar/Desktop/product.png";
	//Mat im = imread("C:/Users/Sankar/Desktop/lena.jpg");
	imReadBasics ob = imReadBasics(path);

	if (!ob.imOrg.data) 
	{
		cout << "Cannot load image!" << endl;
		exit(1);
	}
	
	else
	{
		
		ob.findCanny();
		string ty;
		
		ty =  utils::type2str( ob.imOrg.type() );
		printf("%s \n", ty.c_str());
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imOrg.cols, ob.imOrg.rows );
		
		ty =  utils::type2str( ob.imGry.type() );
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imGry.cols, ob.imGry.rows );
		cout<<ob.imGry.cols * ob.imGry.rows <<endl;
		/*
		cv::Scalar intensity;
		//cv::Point2d pt = cv::Point2d(251,250);
		intensity= ob.imOrg.at<uchar>(cv::Point2d(251,250));
		cout<< intensity<<endl;
		double a = intensity[0] + intensity[1];
		cout<<a<<endl;
		*/
        geometry g = geometry(path);
		g.findCanny();
		//ob.displayImage();
		
		g.xLen = 40;
		g.yLen = 30;
		int i=1;
		while (i != 0)
		{
			if (g.containsVal())
			{
				//cout<<"Found Edge at: "<<g.sectionNo<<endl;
				g.paintMask();
			}
			i = g.computeNextSection();
		}
		//g.displayImage();
        Vec2i point;
        for(i = 0; i < g.interestPoints.size(); i++)
        {
            point = g.interestPoints.at(i);
            cout << "Interest Point: " << point << endl;
        }
        g.arrangeInOrder(g.interestPoints);
        for(i = 0; i < g.interestPoints.size(); i++)
        {
            point = g.interestPoints.at(i);
            cout << "Interest Point: " << point << endl;
        }
        //g.HoughTransformForCircles();
        g.doEverything();
        g.getGLImage(g.normalizedInterestPoints);
	}
}

void main()
{
    //mainT();

    //takeImage();

    //cameraMatInit();

    cameraCalib();

    //cameraCalibTakeImages();

    //glDraws ob = glDraws(800, 800);
    //Mat imG = ob.mainGL(800, 800);
    //imshow("Gl Image", imG);
    //waitKey(0);
}