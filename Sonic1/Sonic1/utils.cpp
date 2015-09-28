#include "utils.h"

using namespace std;
using namespace cv;

utils::utils(void)
{
}

void utils::readFromFile(string fileName, string name, vector<double>& data)
{
    FileStorage fs(fileName, FileStorage::READ);

    fs[name] >> data;
    //fs["distCoeffs"] >> distCoeffs;

    //cout << "camera mat:" << cameraMat << endl;
    //cout << "dist coeffs:" << distCoeffs.at(0) << endl;
    fs.release();
}

void utils::writeIntoFile(string fileName, vector<string> name, vector<vector<double>> data)
{
    FileStorage fs(fileName, FileStorage::WRITE);
    for(int i=0; i < data.size(); i++)
    {
        fs << name.at(i) << data.at(i);
    }

    fs.release();
}

string utils::type2str(int type) 
{
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) 
  {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

//Print Data of an Image
string utils::printData(Mat im)
{
	string ty;
	ty =  utils::type2str( im.type() );
	printf("Matrix: %s %dx%d \n", ty.c_str(), im.cols, im.rows );
	return ty;
}

utils::~utils(void)
{
}