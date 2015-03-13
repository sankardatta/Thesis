#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"

using namespace std;

void main()
{
	//Mat im = imread("C:/Users/Sankar/Desktop/lena.jpg");
	imReadBasics ob = imReadBasics("C:/Users/Sankar/Desktop/product1.png");
	if (!ob.imOrg.data) 
	{
		cout << "Cannot load image!" << endl;
		exit(1);
	}
	else
	{
		/*
		try
		{
			ob.displayImage("Image", ob.imOrg);
		}
		catch()
		{

		}*/
		
		string ty =  ob.type2str( ob.imOrg.type() );
		//printf("%s \n", ty); //Need to print it in C style
		printf("%s \n", ty.c_str());
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imOrg.cols, ob.imOrg.rows );
		
		ty =  ob.type2str( ob.imGry.type() );
		printf("%s \n", ty.c_str());

		ob.findCanny();
		ob.displayImage();
	}
}