#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "imReadBasics.h"

using namespace cv;
using namespace std;

void main()
{
    //Mat im = imread("C:/Users/Sankar/Desktop/lena.jpg");
	imReadBasics ob = imReadBasics("C:/Users/Sankar/Desktop/product.png");
	if (!ob.im.data) 
    {
        cout << "Cannot load image!" << endl;
    }
    imshow("Image", ob.im);
    waitKey(0);
}