#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"
#include "utils.h"
#include "geometry.h"

using namespace std;

void main()
{
	string path = "C:/Users/Sankar/Desktop/product1.png";
	//Mat im = imread("C:/Users/Sankar/Desktop/lena.jpg");
	imReadBasics ob = imReadBasics(path);

	if (!ob.imOrg.data) 
	{
		cout << "Cannot load image!" << endl;
		exit(1);
	}
	
	else
	{
		/*
		ob.findCanny();
		string ty;
		
		ty =  utils::type2str( ob.imOrg.type() );
		//printf("%s \n", ty); //Need to print it in C style for printf; Else use cout
		printf("%s \n", ty.c_str());
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imOrg.cols, ob.imOrg.rows );
		/*
		ty =  utils::type2str( ob.imGry.type() );
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imGry.cols, ob.imGry.rows );
		cout<<ob.imGry.cols * ob.imGry.rows <<endl;
		/*
		cv::Scalar intensity;
		//cv::Point2d pt = cv::Point2d(251,250);
		intensity= ob.imOrg.at<uchar>(cv::Point2d(251,250));
		cout<< intensity<<endl;
		double a = intensity[0] + intensity[1];
		cout<<a<<endl;
		*/
		geometry g = geometry(path);
		g.findCanny();
		//ob.displayImage();
		
		g.xLen = 47;
		g.yLen = 48;
		int i=1;
		while (i != 0)
		{
			if (g.containsVal())
			{
				cout<<"Found Edge at: "<<g.sectionNo<<endl;
				g.paintMask();
			}
			i = g.computeNextSection();
		}
		g.displayImage();
	}
}