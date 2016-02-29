//compile code -> g++ sequential_tracking.cpp -o sequential_tracking.e `pkg-config --cflags --libs opencv`

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/opencv.hpp"
#include <time.h>

using namespace std;
using namespace cv;

string parseInt(int number){//convert ints to string for printing
	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void locateCoordsOfDetectedObject(Mat thresholdImage, Mat &cameraFeed){//locate the coordinates of the detected objects
	bool detectedObj = false;//boolean to keep track of detected objects
	Mat temp;
        Rect rectToBindObject = Rect(0, 0, 0, 0);//Create a rectangle to bind the object
	thresholdImage.copyTo(temp);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);	
	if (contours.size()>0)detectedObj = true;//If the contours are not empty then an object has been detected
	else detectedObj = false;

	if (detectedObj){//when an object is detected		
		vector< vector<Point> > largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));//Find largest contour at end of the contour vector		
		rectToBindObject = boundingRect(largestContourVec.at(0));//Put binding rect around the object
		int xpos = rectToBindObject.x + rectToBindObject.width / 2;//find position x and y of the centre of the object
		int ypos = rectToBindObject.y + rectToBindObject.height / 2;
		//draw circle around object and write coords to screen
		circle(cameraFeed, Point(xpos, ypos), 20, Scalar(0, 255, 0), 2);
		putText(cameraFeed, "(" + parseInt(xpos) + "," + parseInt(ypos) + ")", Point(xpos, ypos), 1, 1, Scalar(255, 0, 0), 2);
	}
}

int main(int, char**)
{
    VideoCapture cap("sherman.mp4");
    if(!cap.isOpened()) 
        return -1;

    Mat edges;
    namedWindow("Tracking",1);
    namedWindow("Threshold",2);

    for(;;)
    {
      Mat frame, frame2, diff, grey1, grey2, thresh;
        cap >> frame;
	cap >> frame2;
	cv::cvtColor(frame, grey1, COLOR_BGR2GRAY);//convert to greyscale
	cv::cvtColor(frame2, grey2, COLOR_BGR2GRAY);//convert to greyscale
        //GaussianBlur(frame, frame, Size(15,15), 3,3);
	//GaussianBlur(frame2, frame2, Size(15,15), 3, 3);
	absdiff(grey1, grey2, diff);
	threshold(diff, thresh, 130, 255, THRESH_BINARY);//(Mat grayScaleImage, Mat outputSave, sensitvityValue, maxVal, typeOutput)

	locateCoordsOfDetectedObject(thresh, frame2);

        imshow("Tracking", frame2);
	        imshow("Threshold", thresh);

        if(waitKey(30) >= 0) break;
    }
    return 0;
}
