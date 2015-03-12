#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"

using namespace std;
using namespace cv;

imReadBasics::imReadBasics(string path)
{
	imOrg = imread(path);
	imGry.create(imOrg.size(), imOrg.type());
}

void imReadBasics::displayImage()
{
	imshow("imOrg", this->imOrg);
	imshow("imGry", this->imGry);
}

void imReadBasics::displayImage(string name, Mat im)
{
	imshow(name, im);
	waitKey(0);
}

void imReadBasics::findCanny()
{
	//imBlur = blur( src_gray, detected_edges, Size(3,3) );
}

imReadBasics::~imReadBasics(void)
{
	imOrg.release();
	imGry.release();
}
