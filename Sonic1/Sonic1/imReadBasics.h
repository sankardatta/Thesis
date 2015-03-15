#pragma once

class imReadBasics
{
public:
	cv::Mat imOrg;
	cv::Mat imGry;

private:
	int lowThreshold, ratio, kernel_size;

protected:
	double cols, rows;
	cv::Mat imDest, imEdges;

public:
	imReadBasics(void);
	imReadBasics(std::string path);
	void displayImage(void);
	void displayImage(cv::Mat, std::string = "Image");
	void findCanny();
	~imReadBasics(void);
	
};

