#include<iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "geometry.h"

using namespace std;
using namespace cv;

geometry::geometry(string path): imReadBasics(path)
{
	xCur = 0;
	yCur = 0;
	sectionNo = 1;
}

geometry::geometry(imReadBasics ob): imReadBasics(ob)
{
	//Does nothing but to initialize imReadBasics with ob
}

bool geometry:: nearbyInterestPoint( int x, int y)
{
    // Returns True in case of existance of a previously
    //detected interest point nearby
    Vec2i point;
    for (int i = 0; i < interestPoints.size(); i ++)
    {
        point = interestPoints.at(i);
        if ((y - point[1]) <= yLen)
        {
            if (abs(point[0] - x) <= xLen)
            {
                return true;
            }
        }
    }
    return false;
}

bool geometry:: containsVal()
{
    // Returns true if the block defined by xCur and yCur
    //contains a value
    int xLimit, yLimit, iterX, iterY ;
	Scalar val;

	xLimit = (xCur + xLen) > cols ? (cols - 1) : (xCur + xLen)-1 ;
	yLimit = (yCur + yLen) > rows ? (rows - 1) : (yCur + yLen)-1 ;

	if (sectionNo == 1)
	{
		if (!imEdges.data)
		{
			findCanny();
		}
	}

	iterX = xCur;
	while (iterX <= xLimit)
	{
		iterY = yCur;
		while (iterY<=yLimit)
		{
			
			val = imEdges.at<uchar>(Point2i(iterX, iterY));
			if(val[0] >= 90)
			{
				//imGry.at<uchar>(Point2d(iterX,iterY))=200;
                if (nearbyInterestPoint(iterX, iterY))
                {
                    return false;
                }
                interestPoints.push_back(Vec2i(iterX, iterY));
                //interestPoints.push_back(Vec2i(iterX, iterY));
				return true;
			}
			
			iterY = iterY + 1;
		}
		iterX = iterX + 1;
	}
	return false;
}

int geometry::computeNextSection()
{ //returns int value for section number. Returns 0 upon reaching end.
	if ((xCur + xLen) >= cols)
	{
		if ((yCur + yLen) >= rows)
		{
			xCur = 0; //reset the values
			yCur = 0;
			sectionNo = 0;
		}
		else
		{
			xCur = 0;
			yCur = yCur + yLen;
			sectionNo = sectionNo + 1;
		}
	}
	else
	{
		xCur = xCur + xLen;
		sectionNo = sectionNo + 1;
	}
	return sectionNo;
}

void geometry::paintMask()
{
	double xLimit, yLimit, iterX, iterY ;

	xLimit = (xCur + xLen) > cols ? (cols - 1) : (xCur + xLen)-1 ;
	yLimit = (yCur + yLen) > rows ? (rows - 1) : (yCur + yLen)-1 ;
	iterX = xCur;
	while (iterX<=xLimit)
	{
		iterY = yCur;
		while (iterY<=yLimit)
		{
			imGry.at<uchar>(Point(iterX, iterY)) = 128;
			iterY = iterY + 1;
		}
		iterX = iterX + 1;
	}
}

void geometry:: resetCur()
{
	xCur = 0;
	yCur = 0;
}

void geometry:: setCur(double xVal, double yVal)
{
	xCur = xVal;
	yCur = yVal;
}

geometry::~geometry(void)
{
}
