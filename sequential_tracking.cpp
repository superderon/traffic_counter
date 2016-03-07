//compile code -> g++ sequential_tracking.cpp -o sequential_tracking.e `pkg-config --cflags --libs opencv`

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "opencv2/opencv.hpp"
#include <time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

int carcount = 0, framecount = 0;
int averageYInBand = 0, newAverageYInBand = 0;


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

  //putText(cameraFeed, "Car Count: " + parseInt(carcount), Point(455,20), 5, 1, Scalar(0, 0, 255), 2);
  //rectangle(cameraFeed, Point(0,420), Point(cameraFeed.cols,430), Scalar(0, 255, 255), 1);//carcount
  
  if (detectedObj){//when an object is detected		
    for (int i = contours.size()-1; i>0; i--)
      {
        vector< vector<Point> > largestContourVec;
	largestContourVec.push_back(contours.at(i));//Find all contours		
	rectToBindObject = boundingRect(largestContourVec.at(0));//Put binding rect around the object
	
	int pt1x = rectToBindObject.x;
	int pt1y = rectToBindObject.y;
	int pt2x = rectToBindObject.x + rectToBindObject.width;
	int pt2y = rectToBindObject.y + rectToBindObject.height;

	int xpos = rectToBindObject.x + rectToBindObject.width / 2;//find position x and y of the centre of the object
	int ypos = rectToBindObject.y + rectToBindObject.height / 2;
	
	if (ypos > 420 && ypos < 430){//carCount
	  newAverageYInBand += ypos;
	}



	rectangle(cameraFeed, Point(pt1x,pt1y), Point(pt2x,pt2y), Scalar(0, 255, 0), 1);
	putText(cameraFeed, "(" + parseInt(xpos) + "," + parseInt(ypos) + ")", Point(xpos, ypos), 1, 1, Scalar(255, 0, 0), 2);
	
      }
    if (averageYInBand < newAverageYInBand){
      carcount+=1;
      //cout<<carcount<<"\n";
    }
    averageYInBand = newAverageYInBand;
	
  }
}

int main(int, char**)
{
  VideoCapture cap("highway.mp4");
  if(!cap.isOpened()) 
    return -1;

  namedWindow("Difference");
  moveWindow("Difference", 10, 0);
  namedWindow("Tracking");
  moveWindow("Tracking", 870, 0);
  //namedWindow("Difference");
  //moveWindow("Difference", 10, 550);
  Mat frame, frame2, diff, grey1, grey2, thresh;
        
  for(;;)
    {
      cap >> frame;
      cap >> frame2;
      framecount ++;
      usleep(10000);
      cv::cvtColor(frame, grey1, COLOR_BGR2GRAY);//convert to greyscale
      cv::cvtColor(frame2, grey2, COLOR_BGR2GRAY);//convert to greyscale
      GaussianBlur(grey1, grey1, Size(15,15),5,5);
      GaussianBlur(grey2, grey2, Size(15,15),5,5);
      absdiff(grey1, grey2, diff);
      threshold(diff, thresh, 50, 255, THRESH_BINARY);//(Mat grayScaleImage, Mat outputSave, sensitvityValue, maxVal, typeOutput)

      // Create a structuring element
      int erosion_size = 5;  
      Mat element = getStructuringElement(cv::MORPH_CROSS,
					  cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
					  cv::Point(erosion_size, erosion_size) );

      dilate(thresh, thresh, element, Point(-1,-1), 5);//input, output, kernal, anchor, iterations
      // erode(thresh, thresh, element, Point(-1,-1), 2);//input, output, kernal, anchor, iterations
      //dilate(thresh, thresh, element, Point(-1,-1), 2);//input, output, kernal, anchor, iterations

      locateCoordsOfDetectedObject(thresh, frame2);

     
      imshow("Difference", diff);
      imshow("Tracking", frame2);
      //imshow("Feed", frame);

      if(waitKey(30) >= 0) break;
    }
  return 0;
}
