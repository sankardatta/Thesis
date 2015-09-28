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

void colorSegmentation::readHsvValues(int hueThres, int satThres)
{
    forefingerLower = vector<double>(3,100);
    forefingerUpper = vector<double>(3,200);
    readFromFile("hsvValues.xml", "Forefinger", forefinger);
    forefingerLower.at(0) = forefinger.at(0) - hueThres;
    forefingerLower.at(1) = forefinger.at(1) - satThres;
    forefingerUpper.at(0) = forefinger.at(0) + hueThres;
    forefingerUpper.at(1) = forefinger.at(1) + satThres;
}

void colorSegmentation::trackColor()
{
    readHsvValues(7,10);
    Moments m;
    Point2f center;//Point2f(250.0, 250.0);
    Scalar color = Scalar(1,100,120);
    windowOpen = true;
    Size kernel= Size(9,9); //should be odd
    while (windowOpen)
    {
        cap >> OrgIm;
        //need to add gaussian blurr
        if(OrgIm.data)

        {
            GaussianBlur(OrgIm, blurredIm, kernel, 3, 3);
            cvtColor(blurredIm, HsvIm, CV_BGR2HSV);
            inRange(HsvIm, forefingerLower, forefingerUpper, im);
            try
            {
                m = moments(im, false);
                center = Point2f(static_cast<float>(m.m10/m.m00), static_cast<float>(m.m01/m.m00));
                circle(OrgIm, center, 4, color);
                imshow("Tracking", im);
                waitKey(1);
            }
            catch(...)
            {
                imshow("Tracking", im);
                waitKey(1);
            }
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