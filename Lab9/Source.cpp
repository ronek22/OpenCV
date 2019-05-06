#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>

using namespace cv;
using namespace std;

#define MAX_COUNT 500
char rawWindow[] = "Raw Video";
char opticalFlowWindow[] = "Optical Flow Window";
char imageFileName[32];
long imageIndex = 0;
char keyPressed;

int main() {
	VideoCapture cap(0);

	double camWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	double camHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

	Mat frame, grayFrames, prevGrayFrame;
	Mat opticalFlow = Mat(camWidth, camHeight, CV_32FC3);

	vector<Point2f> nextPoints;
	vector<Point2f> prevPoints;

	vector<uchar> status;
	vector<float> err;

	bool needToInit = true;

	int i, k;
	Size subPixWinSize(10, 10), winSize(31, 31);
	namedWindow(rawWindow, WINDOW_AUTOSIZE);
	double angle;

	while (true) {
		cap >> frame;
		cvtColor(frame, grayFrames, COLOR_BGR2GRAY);

		if (needToInit) {
			goodFeaturesToTrack(grayFrames, nextPoints, MAX_COUNT, 0.01, 5, Mat());
			needToInit = false;
		}
		else if (!prevPoints.empty()) {
			calcOpticalFlowPyrLK(prevGrayFrame, grayFrames, prevPoints, nextPoints,
				status, err, winSize, 3);




			for (i = k = 0; i < prevPoints.size(); i++) {
				// original frame
				line(frame, nextPoints[i], prevPoints[i], Scalar(255, 255, 255), 1, 1, 0);
				circle(frame, nextPoints[i], 2, Scalar(255, 255, 255), 1, 1, 0);

				// optical flow
				line(opticalFlow, nextPoints[i], prevPoints[i], Scalar(255, 255, 255), 1, 1, 0);
				circle(opticalFlow, nextPoints[i], 1, Scalar(255, 255, 255), 1, 1, 0);

				nextPoints[k++] = nextPoints[i];
			}

			goodFeaturesToTrack(grayFrames, nextPoints, MAX_COUNT, 0.01, 10, Mat());

		}



		swap(prevPoints, nextPoints);
		nextPoints.clear();
		grayFrames.copyTo(prevGrayFrame);

		imshow(rawWindow, frame);
		imshow(opticalFlowWindow, opticalFlow);


		keyPressed = waitKey(10);
		if (keyPressed == 27)
			break;
		else if (keyPressed == 'r')
			opticalFlow = Mat(camWidth, camHeight, CV_32FC3);

	}
}
