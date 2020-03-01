// run using g++ -std=c++11 project2.cpp -o project2 `pkg-config --libs --cflags opencv-3.3.1-dev`
// linking to the version of opencv you have 
// https://answers.ros.org/question/11916/about-compiling-opencv-programs-outside-ros/

#include <opencv2/opencv.hpp>
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME "CVUI Hello World!"

int main(void)
{
	cv::Mat frame = cv::Mat(200, 500, CV_8UC3);
	int count = 0;

	// Init a OpenCV window and tell cvui to use it.
	cv::namedWindow(WINDOW_NAME);
	cvui::init(WINDOW_NAME);

	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);

		// Show a button at position (110, 80)
		if (cvui::button(frame, 110, 80, "Hello, world!")) {
			// The button was clicked, so let's increment our counter.
			count++;
		}

		// Show how many times the button has been clicked.
		// Text at position (250, 90), sized 0.4, in red.
		cvui::printf(frame, 250, 90, 0.4, 0xff0000, "Button click count: %d", count);

		// Update cvui internal stuff
		cvui::update();

		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);

		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
	return 0;
}
