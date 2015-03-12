#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"

using namespace std;

void main()
{
	//Mat im = imread("C:/Users/Sankar/Desktop/lena.jpg");
	imReadBasics ob = imReadBasics("C:/Users/Sankar/Desktop/product.png");
	if (!ob.imOrg.data) 
	{
		cout << "Cannot load image!" << endl;
		exit(1);
	}
	else
	{
		ob.displayImage("Image", ob.imOrg);
		//imshow("Image", ob.imOrg);
		//imshow("Empty", ob.imGry);
	}
}