#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "imReadBasics.h"
#include "utils.h"
#include "geometry.h"

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
		
		ob.findCanny();
		
		string ty;
		ty =  utils::type2str( ob.imOrg.type() );
		//printf("%s \n", ty); //Need to print it in C style for printf; Else use cout
		printf("%s \n", ty.c_str());
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imOrg.cols, ob.imOrg.rows );

		ty =  utils::type2str( ob.imGry.type() );
		printf("Matrix: %s %dx%d \n", ty.c_str(), ob.imGry.cols, ob.imGry.rows );

		cv::Scalar intensity;
		//cv::Point2d pt = cv::Point2d(251,250);
		intensity= ob.imOrg.at<uchar>(cv::Point2d(251,250));
		cout<< intensity<<endl;
		double a = intensity[0] + intensity[1];
		cout<<a;
		
		geometry g = geometry(ob);
		//g.findCanny();
		g.displayImage();
		//ob.displayImage();
	}
}