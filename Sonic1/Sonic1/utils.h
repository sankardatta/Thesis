#pragma once
#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

class utils
{
public:
	utils(void);
	static string type2str(int);
	static string printData(Mat);
    static void writeIntoFile(string fileName, vector<string> name, vector<vector<double>> data);
    static void readFromFile(string fileName, string name, vector<double>& data);
	~utils(void);
};

