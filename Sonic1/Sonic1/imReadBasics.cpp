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
	imshow("imEdges", this->imEdges);
	namedWindow( "imDest", CV_WINDOW_AUTOSIZE );
	//createTrackbar( "Min Threshold:", "imDest", &lowThreshold, 200, this->findCanny );
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
	lowThreshold = 100;
	ratio = 3;
	kernel_size = 3;

	imDest.create(imOrg.size(), imOrg.type());
	cvtColor( imOrg, imGry, CV_BGR2GRAY );
	blur( imGry, imEdges, Size(3,3) );
	Canny( imEdges, imEdges, lowThreshold, lowThreshold*ratio, kernel_size );
	imOrg.copyTo(imDest, imEdges);
}

string imReadBasics::type2str(int type) 
{
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) 
  {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}


imReadBasics::~imReadBasics(void)
{
	imOrg.release();
	imGry.release();
	imEdges.release();
	imDest.release();
}
