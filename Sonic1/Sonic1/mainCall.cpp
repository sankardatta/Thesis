#include <iostream>
#include <fstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include "imReadBasics.h"
#include "utils.h"
#include "geometry.h"
#include "glDraws.h"
#include "glCenturai.h"
#include "camCalib.h"

#include "glModel.h"

using namespace std;
using namespace cv;

vector<Point2f> cameraMatrix(Mat);
Mat cameraCalibInitVal(Mat);
vector<Point2f> obtainImagePoints(Mat);
void writeIntoFile(Mat, vector<double>);

struct myclass
{
    bool operator() (Point3f pt1, Point3f pt2) 
    { 
        return (pt1.x < pt2.x);
    }
}myobject;

vector<Point3f> readFromCOR(string path)
{
    ifstream myFile(path, ios_base::in);
    double val,x,y,z;
    int i=0;
    vector<Point3f> imgFilePoints;

    while(myFile >> val >> x >>y >>z)
    {
        imgFilePoints.push_back(Point3f(x,y,z));
        i++;
    }
    return imgFilePoints;
    }

vector<Point2f> readFromDAT(string path)
{
    ifstream myFile(path, ios_base::in);
    double val,x,y,ex,ey;
    int i=0;
    vector<Point3f> imgFilePoints;
    vector<Point2f> imgPoints;
    //vector<vector<Point2f>> arrayImgPoints;
   
    while(myFile >> val >> x >>y >>ex >>ey)
    {
        //cout<<i<<":"<<val << " " <<x << " "<<y<< " "<<ex<< " "<<ey<< endl;
        imgFilePoints.push_back(Point3f(val,x,y));
        i++;
    }

    sort(imgFilePoints.begin(), imgFilePoints.end(), myobject);
    
    int count = 0;
    while(count<i)
    {
        imgPoints.push_back( Point2f ( imgFilePoints.at(count).y, imgFilePoints.at(count).z));
        count++;
    }
    //arrayImgPoints.push_back(imgPoints);
    //return arrayImgPoints;
    return imgPoints;
}

void genCamMat()
{
    Mat im = imread("C:\\Users\\Sankar\\Desktop\\Calibration\ Reference\\A_Kamera_0.bmp");
    cout << "Size: "<<im.size()<<endl;

    string path = "C:\\Users\\Sankar\\Desktop\\Calibration\ Reference\\passpTest.cor";
    vector<vector<Point3f>> arrayObjectPoints;
    vector<Point3f> objectPoints = readFromCOR(path);
    arrayObjectPoints.push_back(objectPoints);
    
    path = "C:\\Users\\Sankar\\Desktop\\Calibration\ Reference\\Test5.DAT";
    vector<vector<Point2f>> arrayImgPoints;
    vector<Point2f> imgPoints = readFromDAT(path);
    arrayImgPoints.push_back(imgPoints);
    Size sz(1392,1040);
    //Size sz(1040,1392);
    Mat initCameraMat = initCameraMatrix2D(arrayObjectPoints, arrayImgPoints, sz, 1);
    
//    initCameraMat.at<double>(0,0) = 16.97;
//    initCameraMat.at<double>(1,1) = 16.97;
    cout << "initCameraMat: "<< initCameraMat << endl;

    arrayImgPoints.clear();
    arrayObjectPoints.clear();
    path = "C:\\Users\\Sankar\\Desktop\\Calibration\ Reference\\Test";
    string loc;
    for(int i = 0; i < 16 ; i++)
    {
        if (i == 5 || i == 1 || i == 13)
            continue;
        loc = path + to_string(i) + ".DAT";
        imgPoints = readFromDAT(loc);
        if(imgPoints.size()==objectPoints.size())
        {
            cout << i << "th File data inserted." <<endl;
            arrayImgPoints.push_back(imgPoints);
            arrayObjectPoints.push_back(objectPoints);
        }
    }
    vector<double> distCoeffs(2);
    vector<vector<double>> rvecs, tvecs;
    //for(int i=0; i <8; i++) //distCoeffs can have 8 possible values : Refer OpenCV
    //{
    //    distCoeffs.push_back(0.0);
    //}
    TermCriteria criteria=TermCriteria( TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON);
    //getchar();
    double val = calibrateCamera(arrayObjectPoints, arrayImgPoints, sz, initCameraMat, distCoeffs, rvecs, tvecs, CV_CALIB_USE_INTRINSIC_GUESS);  //criteria);

    cout << "Camera Matrix: " << initCameraMat <<endl;
    cout << "DistCoeffs: " << distCoeffs.at(0) << " "<< distCoeffs.at(1) <<endl;
    //cout << "DistCoeffs Size: " << distCoeffs.size() <<endl;

    getchar();
}

void testWithChessBoard()
{
    Mat im = imread("C:\\Users\\Sankar\\Desktop\\ChessBoardCut.jpg");
    while(1)
    {
    CvCapture* capture = cvCaptureFromCAM(0);
    IplImage* frame;
    frame = cvQueryFrame( capture );
    im = Mat(frame);
    Mat gray;
    cvtColor(im, gray, CV_BGR2GRAY);
    Mat dst_norm, dst_norm_scaled;
    Mat dst = Mat::zeros( gray.size(), CV_32FC1 );
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    cornerHarris( gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

    /// Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );

    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
        { for( int i = 0; i < dst_norm.cols; i++ )
            {
            if( (int) dst_norm.at<float>(j,i) > 150 )
                {
                circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
                }
            }
        }

    imshow("Corner",dst_norm_scaled);
    cvWaitKey(1);
    }
}

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


void featureDetection()
{
    //auto start = chrono::high_resolution_clock::now();
    //cvNamedWindow( "mywindow", CV_WINDOW_AUTOSIZE );
    Mat im1 = imread("C:\\Users\\Sankar\\Google\ Drive\\cameraOutput\\original.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat im2 = imread("C:\\Users\\Sankar\\Google\ Drive\\cameraOutput\\training.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    //IplImage* image = cvCloneImage(&(IplImage)im1);
    if(! im1.data || ! im2.data)
    {
        int x;
        cout<< "Couldn't open";
        cin>>x;
        exit(0);
    }
    //imshow( "mywindow", im2 );
    //waitKey(0);

    int minHessian = 400;

    SurfFeatureDetector detector(minHessian);

    vector<KeyPoint> keypoints1, keypoints2;

    detector.detect(im1, keypoints1);
    detector.detect(im2, keypoints2);

    Mat im1_withKeypoints, im2_withKeypoints;

    //drawKeypoints(im1, keypoints1, im1_withKeypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    //drawKeypoints(im2, keypoints2, im2_withKeypoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    //imshow("Keypoints1", im1_withKeypoints);
    //imshow("Keypoints2", im2_withKeypoints);
    //waitKey(0);

    SurfDescriptorExtractor extractor;
    Mat descriptor1, descriptor2;

    extractor.compute( im1, keypoints1, descriptor1);
    extractor.compute( im2, keypoints2, descriptor2);
    if (!descriptor1.data)
    {
        exit(0);
    }

    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptor1, descriptor2, matches);

    double max_dist = 0;
    double min_dist = 10;

    for(int i=0; i<descriptor1.rows; i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist) min_dist = dist;
        if(dist > max_dist) max_dist = dist;
    }

    cout<<"-- Max dist : "<< max_dist << endl ;
    cout<<"-- Min dist : "<< min_dist << endl ;

    vector< DMatch > good_matches;

    for( int i = 0; i < descriptor1.rows; i++ )
    { if( matches[i].distance <= max(3*min_dist, 0.001) )
        { good_matches.push_back( matches[i]); }
    }

    Mat img_matches;
    drawMatches(im1, keypoints1, im2, keypoints2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    /*namedWindow("Matched keys", WINDOW_NORMAL);
    imshow("Matched keys", img_matches);
    waitKey(0);*/

    vector<Point2f> obj;
    vector<Point2f> scene;

    for(int i=0; i<good_matches.size(); i++)
    {
        obj.push_back( keypoints1[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints2[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography(obj, scene, CV_RANSAC);
    cout << H << endl;
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); 
    obj_corners[1] = cvPoint( im1.cols, 0 );
    obj_corners[2] = cvPoint( im1.cols, im1.rows );
    obj_corners[3] = cvPoint( 0, im1.rows );
    std::vector<Point2f> scene_corners(4);
    perspectiveTransform( obj_corners, scene_corners, H);
    line( img_matches, scene_corners[0] + Point2f( im1.cols, 0), scene_corners[1] + Point2f( im1.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f( im1.cols, 0), scene_corners[2] + Point2f( im1.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f( im1.cols, 0), scene_corners[3] + Point2f( im1.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f( im1.cols, 0), scene_corners[0] + Point2f( im1.cols, 0), Scalar( 0, 255, 0), 4 );

    namedWindow("Matched keys", WINDOW_NORMAL);
    imshow("Matched keys", img_matches);
    waitKey(0);

    glModel glOb = glModel();
    glOb.opencvHandler();

}

void main()
{
    featureDetection();

    //glCenturai ob = glCenturai();
    //ob.gameLoop();

    //genCamMat();
    //mainT();
    //takeImage();
    //cameraMatInit();

    //glDraws ob = glDraws(800, 800);//startSolvePNP(); 
    //vector<double> t,r;
    //ob.mainGL(t,r);
    //testWithChessBoard();
    //camCalib oc = camCalib();

    //cameraCalib();

    //cameraCalibTakeImages();

    //glDraws ob = glDraws(800, 800);
    //Mat imG = ob.mainGL(800, 800);
    //imshow("Gl Image", imG);
    //waitKey(0);
}