#pragma once
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

class camCalib
{
public:
    camCalib(void);
    void calibFromImages(string); //path to folder
    
    ~camCalib(void);

private:
    vector<Point2f> obtainImagePoints(Mat);
    Mat cameraCalibInitVal(Mat);
};

