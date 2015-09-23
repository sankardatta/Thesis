#include "colorSegmentation.h"


colorSegmentation::colorSegmentation(void)
{
    cap.open(0);
    cont = false;
    fingerCount = 1;
}

colorSegmentation::colorSegmentation(int id)
{
    cap.open(id);
    cont = false;
    fingerCount = 1;
}

colorSegmentation::~colorSegmentation(void)
{
    
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
        cout<< "Mouse Click"<< endl;
        cout<< "X:"<< x<< "  Y:"<< y<< endl;
        vector<double> v;
        v.push_back((double) x);
        v.push_back((double) y);
        v.push_back(0);
        colorSegmentation * cS = static_cast<colorSegmentation *> (obj);
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
            cS->cont = !(cS->cont);
            cout << "\n CONT:" << cS->cont << endl;
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
        char x;
        cout<<"CameraCap is not open (press key to exit):"<<endl;
        cin >> x;
    }

    while(true)
    {
        
        if(cont)
        {
            OrgIm.copyTo(im);

            switch (fingerCount)
            {
            case 1:
                putText(im, "Select Forefinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                break;
            case 2:
                putText(im, "Select Middlefinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                break;
            case 3:
                putText(im, "Select Ringfinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                break;
            case 4:
                putText(im, "Select Smallfinger", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                break;
            case 5:
                putText(im, "Select Thumb", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                break;
            default:
                writeIntoFile("hsvValues.xml", name, data);
                fingerCount = 1;
                cont = !cont;
                break;
            }
            //putText(im, "Select Location", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
            imshow("HSV", im);
            setMouseCallback("HSV",  hsvSelection, this );
            waitKey(1);
            //setMouseCallback("My Window",  onMouse, this );
        }
        else
        {
            cap >> OrgIm;
            OrgIm.copyTo(im);
            fingerCount = 1;

            if(im.data)
            {
                putText(im, "Hello", p, FONT_HERSHEY_SIMPLEX, fontScale, color, thickness, lineType, false);
                imshow("Live", im);
                setMouseCallback("Live",  onMouse, this );
                waitKey(1);
                //setMouseCallback("My Window",  onMouse, this );
                //waitKey(10);
                //cout<<"\npress c to capture:";
                //cin >> x;
                if ( (cvWaitKey(10) & 255) == 's' )
                {
                    imwrite("C:\\Users\\Sankar\\Desktop\\matteo.jpg", im);
                    exit(0);
                }
            
            }
        }
        
    }
}