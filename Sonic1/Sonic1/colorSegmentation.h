#pragma once
#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"

using namespace std;
using namespace cv;

class colorSegmentation
{
public:
    colorSegmentation(void);
    colorSegmentation(int);
    void trackColor();
    void readFrame(void);
    ~colorSegmentation(void);

private:
    static void onMouse(int event, int x, int y, int, void*);
    static void tracking(int event, int x, int y, int, void*);
    static void hsvSelection(int event, int x, int y, int, void*);

private:
    //void writeIntoFile(string fileName, vector<string> name);//, vector<vector<double>> data);
    void writeIntoFile(string fileName, vector<string> name, cv::vector<cv::vector<double>>);
    void readFromFile(string fileName, string name, vector<double>& data);
    void readHsvValues(int hueThres, int satThres);
    float pixelDistance(Point2f, Point2f);
    double findYOrientation(Point2f, Point2f, Point2f, Point2f);
    float distanceEstimator(Point2f, Point2f, Point2f, Point2f);

public:
    Mat im;
    Mat OrgIm, blurredIm;
    Mat HsvIm;

private:
    bool cont, windowOpen;
    int fingerCount;
    vector<double> forefinger, forefingerLower, forefingerUpper;
    vector<double> thumb, thumbUpper, thumbLower;
    vector<double> smallfinger, smallfingerLower, smallfingerUpper;
    vector<double> middlefinger, middlefingerLower, middlefingerUpper;
    vector<double> ringfinger, ringfingerLower, ringfingerUpper;
    vector<string> name;
    vector<vector<double>> data;
    Point clickLocation;
    VideoCapture cap;
};

