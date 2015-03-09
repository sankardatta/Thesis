#include<iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"



using namespace std;
using namespace cv;

imReadBasics::imReadBasics(string path)
{
	im = imread(path);
}


imReadBasics::~imReadBasics(void)
{
}
