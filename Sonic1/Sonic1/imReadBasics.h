#pragma once

class imReadBasics
{

public:
	imReadBasics(std::string path);
	~imReadBasics(void);
	cv::Mat im;
};

