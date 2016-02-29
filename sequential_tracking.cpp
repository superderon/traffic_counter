//compile code -> g++ sequential_tracking.cpp -o sequential_tracking.e `pkg-config --cflags --libs opencv`

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/opencv.hpp"
#include <time.h>

using namespace std;
using namespace cv;


int main(int, char**)
{
    VideoCapture cap("sherman.mp4");
    if(!cap.isOpened()) 
        return -1;

    Mat edges;
    namedWindow("Difference",1);
    for(;;)
    {
      Mat frame, frame2, diff;
        cap >> frame;
	cap >> frame2;
        //GaussianBlur(frame, frame, Size(15,15), 3,3);
	//GaussianBlur(frame2, frame2, Size(15,15), 3, 3);
	absdiff(frame, frame2, diff);
        imshow("Difference", diff);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
