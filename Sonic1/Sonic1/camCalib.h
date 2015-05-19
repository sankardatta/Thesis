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
    void readCalibFile(Mat&, vector<double>&);
    void writeIntoFile(Mat, vector<double>);
    void solvePNPFromCamera(Mat ,vector<double> );
    ~camCalib(void);

private:
    void genCamMatrix();
    Mat cameraCalibInitVal(Mat);
    vector<Point2f> cameraMatrixFromChessBoard(Mat);
    vector<Point2f> obtainImagePoints(Mat);
};

