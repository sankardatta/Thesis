#include "colorSegmentation.h"


colorSegmentation::colorSegmentation(void)
{
    cap.open(0);
    
    if(!cap.isOpened())
    {
        char c;
        cout<< "Unable to open Device number "<< 0 << ". (Press any key to exit)" << endl;
        cin >> c;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 640);

    cont = false;
    windowOpen = true;
    fingerCount = 1;
    im = Mat(1080,640,CV_32F); //CV_32F needed for gaussian blur
    OrgIm = Mat(im.cols,im.rows,im.type());
}

colorSegmentation::colorSegmentation(int id=0)
{
    cap.open(id);
    if(!cap.isOpened())
    {
        char c;
        cout<< "Unable to open Device number "<< id << ". (Press any key to exit)" << endl;
        cin >> c;
    }
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 640);

    cont = false;
    fingerCount = 1;
    im = Mat(1080,640,CV_32F);
    OrgIm = Mat(im.cols,im.rows,im.type());
}

colorSegmentation::~colorSegmentation(void)
{
    
}

void colorSegmentation::readFromFile(string fileName, string name, vector<double>& data)
{
    FileStorage fs(fileName, FileStorage::READ);

    fs[name] >> data;
    //fs["distCoeffs"] >> distCoeffs;

    //cout << "camera mat:" << cameraMat << endl;
    //cout << "dist coeffs:" << distCoeffs.at(0) << endl;
    fs.release();
}

void colorSegmentation::writeIntoFile(string fileName, vector<string> name, cv::vector<cv::vector<double>> data)
{
    FileStorage fs(fileName, FileStorage::WRITE);
    for(int i=0; i < name.size(); i++)
    {
        cout << name.at(i) << endl;
        fs << name.at(i) << data.at(i);//data.at(i);
    }

    fs.release();
}

//void colorSegmentation::writeIntoFile(string fileName, vector<string> name)//, vector<vector<double>> data)
//{
//    FileStorage fs(fileName, FileStorage::WRITE);
//    for(int i=0; i < name.size(); i++)
//    {
//        cout << name.at(i) << endl;
//        fs << name.at(i) << vector<double>(3);//data.at(i);
//    }
//
//    fs.release();
//}

void colorSegmentation::hsvSelection(int event, int x, int y, int flags, void* obj)
{
    if(event == EVENT_LBUTTONDOWN)
    {
        colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
        vector<double> v;
        double hue = cS->HsvIm.at<Vec3b>(y, x)[0];
        double sat = cS->HsvIm.at<Vec3b>(y, x)[1];
        double val = cS->HsvIm.at<Vec3b>(y, x)[2];
        v.push_back((double) hue);
        v.push_back((double) sat);
        v.push_back((double) val);
        
        cout << "\n finger:" << cS->fingerCount << endl;
        switch (cS->fingerCount)
        {
        case 1:
            cS->name.push_back("Forefinger");
            cS->data.push_back(v);
            break;
        case 2:
            cS->name.push_back("Middlefinger");
            cS->data.push_back(v);
            break;
        case 3:
            cS->name.push_back("Ringfinger");
            cS->data.push_back(v);
            break;
        case 4:
            cS->name.push_back("Smallfinger");
            cS->data.push_back(v);
            break;
        case 5:
            cS->name.push_back("Thumb");
            cS->data.push_back(v);
            break;
        default:
            cS->name.push_back("Error");
            cS->data.push_back(v);
            break;
        }

        cS->fingerCount = cS->fingerCount + 1;

    }
    
    else if(event == EVENT_RBUTTONDOWN)
        {
            colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
            cS->name.clear();
            cS->data.clear();
            cvDestroyWindow("HSV");
            cS->cont = !(cS->cont);
            cout << "\n CONT:" << cS->cont << endl;
        }
}

void colorSegmentation::tracking(int event, int x, int y, int flags, void* obj)
{
    if(event == EVENT_RBUTTONDOWN)
    {
        colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
        cS->windowOpen = false;
    }
}

void colorSegmentation::onMouse(int event, int x, int y, int flags, void* obj)
{
    if(event == EVENT_LBUTTONDOWN)
        {
            cout<< "Mouse Click"<< endl;
            cout<< "X:"<< x<< "  Y:"<< y<< endl;
            colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
            cS->cont = !(cS->cont);
            cout << "\n CONT:" << cS->cont << endl;
        }
    else if(event == EVENT_RBUTTONDOWN)
    {
        colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
        cS->windowOpen = false;
    }
}

double colorSegmentation::findYOrientation(Point2f A1, Point2f A2, Point2f B1, Point2f B2) //A and B are the two st lines. A1 and A2 are the two ends of the st line
{
    float lenA = pixelDistance(A1, A2);
    float lenB = pixelDistance(B1, B2);
    float dist = distanceEstimator(A1, A2, B1, B2);
    double f_x = 623.7274584149;
    float d1 = f_x * (300/lenA - 1); //considering the real world length of the straight line is 300pixels.
    float d2 = f_x * (300/lenB - 1);
    float c = d1- d2;
    double angle = atan(c / dist) * 180/3.14159265;
    return angle;
}

float colorSegmentation::distanceEstimator(Point2f A1, Point2f A2, Point2f B1, Point2f B2)
{
    Point2f A, B;
    A.x = (A1.x + A2.x)/2;
    A.y = (A1.y + A2.y)/2;
    B.x = (B1.x + B2.x)/2;
    B.y = (B1.y + B2.y)/2;
    return pixelDistance(A,B);
}

float colorSegmentation:: pixelDistance(Point2f a, Point2f b)
{
    float dist = pow((a.x - b.x), 2) + pow((a.y - b.y), 2);
    dist = sqrt(dist);
    return dist;
}

void colorSegmentation::readHsvValues(int hueThres, int satThres)
{
    forefingerLower = vector<double>(3,100);
    forefingerUpper = vector<double>(3,210);

    smallfingerLower = vector<double>(3,100);
    smallfingerUpper = vector<double>(3,210);
    
    middlefingerLower = vector<double>(3,100);
    middlefingerUpper = vector<double>(3,210);
    
    ringfingerLower = vector<double>(3,100);
    ringfingerUpper = vector<double>(3,210);

    readFromFile("hsvValues.xml", "Forefinger", forefinger);
    forefingerLower.at(0) = forefinger.at(0) - hueThres;
    forefingerLower.at(1) = forefinger.at(1) - satThres;
    forefingerUpper.at(0) = forefinger.at(0) + hueThres;
    forefingerUpper.at(1) = forefinger.at(1) + satThres;

    readFromFile("hsvValues.xml", "Smallfinger", smallfinger);
    smallfingerLower.at(0) = smallfinger.at(0) - hueThres;
    smallfingerLower.at(1) = smallfinger.at(1) - satThres;
    smallfingerUpper.at(0) = smallfinger.at(0) + hueThres;
    smallfingerUpper.at(1) = smallfinger.at(1) + satThres;

    readFromFile("hsvValues.xml", "Middlefinger", middlefinger);
    middlefingerLower.at(0) = middlefinger.at(0) - hueThres;
    middlefingerLower.at(1) = middlefinger.at(1) - satThres;
    middlefingerUpper.at(0) = middlefinger.at(0) + hueThres;
    middlefingerUpper.at(1) = middlefinger.at(1) + satThres;

    readFromFile("hsvValues.xml", "Ringfinger", ringfinger);
    ringfingerLower.at(0) = ringfinger.at(0) - hueThres;
    ringfingerLower.at(1) = ringfinger.at(1) - satThres;
    ringfingerUpper.at(0) = ringfinger.at(0) + hueThres;
    ringfingerUpper.at(1) = ringfinger.at(1) + satThres;

}

void colorSegmentation::trackColor()
{
    try
    {
        readHsvValues(7,10);
    }
    catch(...)
    {
        cout<< "Unable to read HSV file." << endl;
        cin.ignore();
        exit(0);
    }
    Moments m;
    Point2f centerForefinger, centerSmallfinger, centerMiddlefinger, centerRingfinger;//Point2f(250.0, 250.0);
    Scalar colorForefinger = Scalar(1,200,180);
    Scalar colorSmallFinger = Scalar(100,150,220);
    Scalar colorMiddleFinger = Scalar(200,120,20);
    Scalar colorRingFinger = Scalar(22,220,100);
    windowOpen = true;
    Size kernel= Size(9,9); //should be odd for gaussian
    Mat trackedIm;
    while (windowOpen)
    {
        cap >> OrgIm;
        //need to add gaussian blurr
        if(OrgIm.data)

        {
            GaussianBlur(OrgIm, blurredIm, kernel, 3, 3);
            cvtColor(blurredIm, HsvIm, CV_BGR2HSV);

            //#1
            inRange(HsvIm, forefingerLower, forefingerUpper, im);
            im.copyTo(trackedIm);
            try
            {
                m = moments(im, false);
                centerForefinger = Point2f(static_cast<float>(m.m10/m.m00), static_cast<float>(m.m01/m.m00));
                circle(OrgIm, centerForefinger, 4, colorForefinger);
            }
            catch(...)
            {
                cout << "\n Failed to track forefinger. \n";
            }
            //#2
            inRange(HsvIm, smallfingerLower, smallfingerUpper, im);
            trackedIm = trackedIm + im;
            try
            {
                m = moments(im, false);
                centerSmallfinger = Point2f(static_cast<float>(m.m10/m.m00), static_cast<float>(m.m01/m.m00));
                circle(OrgIm, centerSmallfinger, 4, colorSmallFinger);
            }
            catch(...)
            {
                cout << "\n Failed to track smallfinger. \n";
            }
            //#3
            inRange(HsvIm, middlefingerLower, middlefingerUpper, im);
            trackedIm = trackedIm + im;
            try
            {
                m = moments(im, false);
                centerMiddlefinger = Point2f(static_cast<float>(m.m10/m.m00), static_cast<float>(m.m01/m.m00));
                circle(OrgIm, centerMiddlefinger, 4, colorMiddleFinger);
            }
            catch(...)
            {
                cout << "\n Failed to track middlefinger. \n";
            }
            //#4
            inRange(HsvIm, ringfingerLower, ringfingerUpper, im);
            trackedIm = trackedIm + im;
            try
            {
                m = moments(im, false);
                centerRingfinger = Point2f(static_cast<float>(m.m10/m.m00), static_cast<float>(m.m01/m.m00));
                circle(OrgIm, centerRingfinger, 4, colorRingFinger);
            }
            catch(...)
            {
                cout << "\n Failed to track Ringfinger. \n";
            }

            //cout << "Distance1: " << pixelDistance(centerForefinger, centerMiddlefinger) << endl;
            //cout << "Distance2: " << pixelDistance(centerRingfinger, centerSmallfinger) << endl;
            cout << "Angle: " << findYOrientation(centerForefinger, centerMiddlefinger, centerRingfinger, centerSmallfinger) << endl;
            imshow("Tracking", trackedIm);
            waitKey(1);
            //circle(OrgIm, center, 4, color);
            imshow("Live", OrgIm);
            setMouseCallback("Tracking",  tracking, this);
            waitKey(1);
        }
    }
    cvDestroyAllWindows();
}

void colorSegmentation::readFrame()
{
    Point p = Point(20,60);
    Scalar color = Scalar(100,100,180);
    double fontScale = 1.0;
    int thickness = 2;
    int lineType = 8;
    namedWindow("Live", 1);
    namedWindow("HSV", 1);
    if (!cap.isOpened())
    {
        cout<<"CameraCap is not open (press key to exit):"<<endl;
        cin.ignore();
    }

    while(windowOpen)
    {
        
        if(cont)
        {
            OrgIm.copyTo(im);
            Size kernel= Size(11,11); //should be odd
            GaussianBlur(OrgIm, blurredIm, kernel, 3, 3);
            cvtColor(blurredIm, HsvIm, CV_BGR2HSV);
            switch (fingerCount)
            {
            case 1:
                putText(blurredIm, "Select Forefinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("HSV", blurredIm);
                setMouseCallback("HSV",  hsvSelection, this );
                waitKey(1);
                break;
            case 2:
                putText(blurredIm, "Select Middlefinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("HSV", blurredIm);
                setMouseCallback("HSV",  hsvSelection, this );
                waitKey(1);
                break;
            case 3:
                putText(blurredIm, "Select Ringfinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("HSV", blurredIm);
                setMouseCallback("HSV",  hsvSelection, this );
                waitKey(1);
                break;
            case 4:
                putText(blurredIm, "Select Smallfinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("HSV", blurredIm);
                setMouseCallback("HSV",  hsvSelection, this );
                waitKey(1);
                break;
            case 5:
                putText(blurredIm, "Select Thumb", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("HSV", blurredIm);
                setMouseCallback("HSV",  hsvSelection, this );
                waitKey(1);
                break;
            default:
                writeIntoFile("hsvValues.xml", name, data);
                destroyWindow("HSV");
                fingerCount = 1;
                cont = !cont;
                break;
            }
            //putText(im, "Select Location", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);

            //setMouseCallback("My Window",  onMouse, this );
        }
        else
        {
            cap >> OrgIm;
            OrgIm.copyTo(im);
            fingerCount = 1;

            if(OrgIm.data)
            {
                putText(im, "Hello", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("Live", im);
                setMouseCallback("Live",  onMouse, this );
                waitKey(1);
                if ( (cvWaitKey(10) & 255) == 's' )
                {
                    imwrite("C:\\Users\\Sankar\\Desktop\\matteo.jpg", im);
                    exit(0);
                }
            
            }
        }
    }

    //vector<double> data;
    //readFromFile("hsvValues.xml", "Forefinger", data);
    //cout << "Forefinger Value: " << *data.data() <<endl << "Press key to exit- ";
    //cin.ignore();

    trackColor();
}