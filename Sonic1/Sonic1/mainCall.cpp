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

void cameraCalib()
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
        if ( (cvWaitKey(10) & 255) == 's' ) {
            size = cvGetSize(frame);
            img = cvCreateImage(size, IPL_DEPTH_16S, 1);
            img = frame;

            loc = path + "Logitech" + to_string(count) + ".jpg";
            count = count + 1;
            cvSaveImage(loc.c_str(), img);
                                            }
		if ( (cvWaitKey(10) & 255) == 'e' ) 
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

    cameraCalib();

    //glDraws ob = glDraws(800, 800);
    //Mat imG = ob.mainGL(800, 800);
    //imshow("Gl Image", imG);
    //waitKey(0);
}