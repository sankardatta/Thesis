#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"

using namespace std;
using namespace cv;

imReadBasics::imReadBasics(string path)
{
	imOrg = imread(path);
	//imGry.create(imOrg.size(), imOrg.type());
}

void imReadBasics::displayImage(void)
{
	imshow("imOrg", this->imOrg);
	imshow("imGry", this->imGry);
	imshow("imEdges", this->imEdges);
	namedWindow( "imDest", CV_WINDOW_AUTOSIZE );
	//createTrackbar( "Min Threshold:", "imDest", &lowThreshold, 200, this->findCanny );
	imshow("imDest", this->imDest);
	
	waitKey(0);
}

void imReadBasics::displayImage(Mat im, string name)
{
	imshow(name, im);
	waitKey(0);
}

void imReadBasics::findCanny()
{
	lowThreshold = 100;
	ratio = 3;
	kernel_size = 3;

	imDest.create(imOrg.size(), imOrg.type());
	cvtColor( imOrg, imGry, CV_BGR2GRAY );
	blur( imGry, imEdges, Size(3,3) );
	Canny( imEdges, imEdges, lowThreshold, lowThreshold*ratio, kernel_size );
	imOrg.copyTo(imDest, imEdges);
}

imReadBasics::~imReadBasics(void)
{
	imOrg.release();
	imGry.release();
	imEdges.release();
	imDest.release();
}
