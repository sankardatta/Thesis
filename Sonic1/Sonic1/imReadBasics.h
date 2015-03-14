#pragma once

class imReadBasics
{
public:
	cv::Mat imOrg;
	cv::Mat imGry;

private:
	cv::Mat imDest, imEdges;
	int lowThreshold, ratio, kernel_size;

public:
	imReadBasics(std::string path);
	void displayImage(void);
	void displayImage(cv::Mat, std::string = "Image");
	void findCanny();
	~imReadBasics(void);
	
};

