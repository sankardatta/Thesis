#pragma once

class imReadBasics
{
public:
	cv::Mat imOrg;
	cv::Mat imGry;

private:
	cv::Mat imDest, imEdges;

public:
	imReadBasics(std::string path);
	void displayImage(void);
	void displayImage(std::string, cv::Mat);
	void findCanny(void);
	~imReadBasics(void);
	
	
};

