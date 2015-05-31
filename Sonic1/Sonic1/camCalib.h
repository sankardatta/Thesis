#pragma once
#include <iostream>
#include "glDraws.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

class camCalib
{
public:
    Mat rotMat, transMat;
    string filename;
    string path;
private:
    vector<Point3f> objectPoints;

public:
    camCalib(void);
    void calibFromImages(string); //path to folder
    void readCalibFile(Mat&, vector<double>&);
    void writeIntoFile(Mat, vector<double>);
    void solvePNPFromImage(Mat ,vector<double> );
    void solvePNPFromCamera(Mat ,vector<double> );
    void prepareData(vector<double>, vector<double>);
    ~camCalib(void);

private:
    void cameraCalibTakeImages(void);
    void genCamMatrix();
    Mat cameraCalibInitVal(Mat);
    vector<Point2f> cameraMatrixFromChessBoard(Mat);
    vector<Point2f> obtainImagePoints(Mat);
    void genObjectPoints(void);
};

