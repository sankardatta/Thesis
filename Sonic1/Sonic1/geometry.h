#pragma once
#include "imReadBasics.h"

class geometry: public imReadBasics
{

public:
	cv::Mat imSegment;
	double xLen, yLen;
	int sectionNo;
    vector<Vec2i> interestPoints; //For usage check: http://goo.gl/FGyRnv

private:
	double xCur, yCur;
    bool nearbyInterestPoint(int, int);

public:
	geometry(std::string);
	geometry(imReadBasics);
	void resetCur(void);
	void setCur(double, double);
	bool containsVal();
	void paintMask();
	int computeNextSection();
	~geometry(void);
};

