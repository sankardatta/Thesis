#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"
#include "imReadBasics.h"
#include "glDraws.h"

using namespace std;
using namespace cv;

imReadBasics::imReadBasics(string path)
{
	imOrg = imread(path);
	cols = imOrg.cols;
	rows = imOrg.rows;
    cvtColor( this->imOrg, this->imGry, CV_BGR2GRAY );
    //interestPoints
	//imGry.create(imOrg.size(), imOrg.type());
}
Mat imReadBasics::addImages(Mat im1, Mat im2, int depth)
{
    double alpha = 0.5;
    double beta = 1 - alpha;
    Mat addedIm;
    utils::printData(im1);
    utils::printData(im2);
    //Mat addedIm(Size(rows, cols), CV_16U);
    addWeighted( im1, alpha, im2, beta, 0.0, addedIm);
    /*for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
            addedIm.at<uchar>(Point2d(i,j)) = im1.at<uchar>(Point2d(i,j)) + im2.at<uchar>(Point2d(i,j));
     }*/

    return addedIm;
}


void imReadBasics::displayImage(void)
{
	if(imOrg.data)
	{
		imshow("imOrg", this->imOrg);
	}

	if(imGry.data)
	{
		imshow("imGry", this->imGry);
	}
	if(imEdges.data)
	{
		imshow("imEdges", this->imEdges);
		namedWindow( "imDest", CV_WINDOW_AUTOSIZE );
		//createTrackbar( "Min Threshold:", "imDest", &lowThreshold, 200, this->findCanny );
		imshow("imDest", this->imDest);
	}

	waitKey(0);
}

void imReadBasics::displayImage(Mat im, string name)
{
	imshow(name, im);
	waitKey(0);
}

void imReadBasics::getGLImage(vector<Vec2f> points)
{
    Mat glImg, addedIm;
    glDraws ob = glDraws(imOrg.cols, imOrg.rows);
    glImg = ob.mainGL(rows, cols, points);
    cvtColor( glImg, glImg, CV_BGR2GRAY );
    addedIm = addImages(glImg, imGry, 1);
    displayImage(glImg, "OpenGL image");
    displayImage(imOrg, "Original");
    displayImage(addedIm, "OpenGL added image");
}

void imReadBasics::getGLImage()
{
    Mat glImg, addedIm;
    glDraws ob = glDraws(imOrg.cols, imOrg.rows);
    glImg = ob.mainGL(rows, cols);
    cvtColor( glImg, glImg, CV_BGR2GRAY );
    cout<<"Image Obtained from OPENGL"<<endl;
    addedIm = addImages(glImg, imGry, 1);
    displayImage(glImg, "OpenGL image");
    displayImage(imOrg, "Original");
    displayImage(addedIm, "OpenGL added image");

}

void imReadBasics::findCanny()
{
	lowThreshold = 100;
	ratio = 3;
	kernel_size = 3;

	imDest.create(imOrg.size(), imOrg.type());
	//blur( imGry, imEdges, Size(3,3) );
    imEdges = blurify(imGry, Size(3,3));
	Canny( imEdges, imEdges, lowThreshold, lowThreshold*ratio, kernel_size );
	cout<<"Rows: "<<imEdges.rows<<"\n"<<"Cols: "<<imEdges.cols<<endl;
	imOrg.copyTo(imDest, imEdges);
}

Mat imReadBasics::blurify(Mat sourceIm, Size kernelsize)
{
    Mat blurIm;
    blur( sourceIm, blurIm, kernelsize );
    return blurIm;
}

void imReadBasics::HoughTransformForCircles()
{
    vector<Vec3f> circles;
    Mat im;
    cvtColor( this->imOrg, im, CV_BGR2GRAY );
    im = blurify(im, Size(3,3));
    HoughCircles( im, circles, CV_HOUGH_GRADIENT, 1, im.rows/8, 200, 100, 0, 0 );
    cout << "Circles:" << circles.size() <<endl;
    for( size_t i = 0; i < circles.size(); i++ )
    {
      Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
      int radius = cvRound(circles[i][2]);
      cout << "Drawing circle No:" << i <<endl;
      // circle center
      circle( imOrg, center, 3, Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( imOrg, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    displayImage(imOrg, "Hough");
}

imReadBasics::~imReadBasics(void)
{
	imOrg.release();
	imGry.release();
	imEdges.release();
	imDest.release();
}
