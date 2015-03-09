#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"



using namespace std;
using namespace cv;

imReadBasics::imReadBasics(void)
{
	im = imread("C:/Users/Sankar/Desktop/lena.jpg");
}


imReadBasics::~imReadBasics(void)
{
}
