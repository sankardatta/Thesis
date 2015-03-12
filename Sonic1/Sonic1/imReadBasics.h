#pragma once

class imReadBasics
{
public:
	cv::Mat imOrg;
	cv::Mat imGry;

private:
	cv::Mat imBlur, imEdges;

public:
	imReadBasics(std::string path);
	void displayImage(void);
	void displayImage(std::string, cv::Mat);
	~imReadBasics(void);

private:
	void findCanny(void);
	
};

