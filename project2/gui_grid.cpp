// run using g++ -std=c++11 project2.cpp -o project2 `pkg-config --libs --cflags opencv-3.3.1-dev`
// linking to the version of opencv you have 
// https://answers.ros.org/question/11916/about-compiling-opencv-programs-outside-ros/

#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "CVUI Hello World!"
using namespace cv;

void MyLine( Mat img, Point start, Point end )
{
  int thickness = 0.1;
  int lineType = LINE_8;
  line( img,
    start,
    end,
    Scalar( 105,105,105),
    thickness,
    lineType );
}

int main(void)
{
	int width = 1200;
	int height = 800;
	Mat frame = cv::Mat(height, width, CV_8UC3, Scalar(255,255,255));
	int division = 3;
	for (int i = 1; i < width/division; i++){
		MyLine(frame, Point(division*i,0), Point(division*i, height));
	}
	for (int i = 1; i < height/division; i++){
		MyLine(frame, Point(0, i*division), Point(width, i*division));
	}
	imshow(WINDOW_NAME, frame);
	waitKey(0);
	// int count = 0;

	// // Init a OpenCV window and tell cvui to use it.
	// cv::namedWindow(WINDOW_NAME);
	// cvui::init(WINDOW_NAME);

	// while (true) {
	// 	// Fill the frame with a nice color
	// 	frame = cv::Scalar(49, 52, 49);

	// 	// Show a button at position (110, 80)
	// 	if (cvui::button(frame, 110, 80, "Hello, world!")) {
	// 		// The button was clicked, so let's increment our counter.
	// 		count++;
	// 	}

	// 	// Show how many times the button has been clicked.
	// 	// Text at position (250, 90), sized 0.4, in red.
	// 	cvui::printf(frame, 250, 90, 0.4, 0xff0000, "Button click count: %d", count);

	// 	// Update cvui internal stuff
	// 	cvui::update();

	// 	// Show everything on the screen
	// 	cv::imshow(WINDOW_NAME, frame);

	// 	// Check if ESC key was pressed
	// 	if (cv::waitKey(20) == 27) {
	// 		break;
	// 	}
	// }
	return 0;
}
