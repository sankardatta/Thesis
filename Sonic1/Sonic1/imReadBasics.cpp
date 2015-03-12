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

void imReadBasics::displayImage()
{
	imshow("imOrg", this->imOrg);
	imshow("imGry", this->imGry);
	namedWindow( "imDest", CV_WINDOW_AUTOSIZE );
	imshow("imDest", this->imDest);
	waitKey(0);
}

void imReadBasics::displayImage(string name, Mat im)
{
	imshow(name, im);
	waitKey(0);
}

void imReadBasics::findCanny()
{
	int lowThreshold = 100, ratio = 3, kernel_size = 3;
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
}
