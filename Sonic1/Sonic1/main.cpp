#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"
#include "utils.h"

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
		
		string ty =  utils::type2str( ob.imOrg.type() );
		//printf("%s \n", ty); //Need to print it in C style
		printf("%s \n", ty.c_str());
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imOrg.cols, ob.imOrg.rows );
		
		ty =  utils::type2str( ob.imGry.type() );
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imGry.cols, ob.imGry.rows );

		ob.findCanny();
		ob.displayImage();
	}
}