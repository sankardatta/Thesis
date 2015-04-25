#pragma once

using namespace cv;

class imReadBasics
{
public:
	Mat imOrg;
	Mat imGry;

private:
	int lowThreshold, ratio, kernel_size;

protected:
	double cols, rows;
	Mat imDest, imEdges;

private:
    IplImage * addImages(IplImage * im1, IplImage * im2, int depth);
    Mat addImages(Mat im1, Mat im2, int depth);

public:
	imReadBasics(void);
	imReadBasics(std::string path);
	void displayImage(void);
	void displayImage(cv::Mat, std::string = "Image");
	void findCanny();
    Mat blurify(Mat sourceIm, Size kernelsize);
    void getGLImage(void);
    void HoughTransformForCircles();
	~imReadBasics(void);
	
};

