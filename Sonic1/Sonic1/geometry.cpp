#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "geometry.h"

using namespace std;

geometry::geometry(string path): imReadBasics(path)
{
	cout<<"Hello";
}

geometry::geometry(imReadBasics ob): imReadBasics(ob)
{
	cout<<"this One";
}

geometry::~geometry(void)
{
}
