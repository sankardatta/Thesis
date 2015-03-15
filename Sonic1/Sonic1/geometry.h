#pragma once
#include "imReadBasics.h"

class geometry: public imReadBasics
{

public:
	cv::Mat imSegment;
	double xLen, yLen;
	int sectionNo;

private:
	double xCur, yCur;

public:
	geometry(std::string);
	geometry(imReadBasics);
	void resetCur(void);
	void setCur(double, double);
	bool containsVal();
	int computeNextSection();
	~geometry(void);
};

