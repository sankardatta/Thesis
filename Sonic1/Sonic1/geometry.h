#pragma once
#include "imReadBasics.h"

class geometry: public imReadBasics
{

public:
	cv::Mat imSegment;
	double xLen, yLen;
	int sectionNo;
    vector<Vec2i> interestPoints; //For usage check: http://goo.gl/FGyRnv
    vector<Vec2f> normalizedInterestPoints;

private:
	double xCur, yCur;
    bool nearbyInterestPoint(int, int);
    double distance(Vec2i, Vec2i);

public:
	geometry(std::string);
	geometry(imReadBasics);
    void doEverything();
    void normalizeInterestPoints();
    void arrangeInOrder(vector<Vec2i> &);
	void resetCur(void);
	void setCur(double, double);
	bool containsVal();
	void paintMask();
	int computeNextSection();
	~geometry(void);
};

