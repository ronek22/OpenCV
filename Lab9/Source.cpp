#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <conio.h>

using namespace cv;
using namespace std;

char rawWindow[] = "Raw Video";
char opticalFlowWindow[] = "Optical Flow Window";
char keyPressed;

int maxCorners = 100;
int qualityLevel = 1;
int minDistance = 5;
bool changed = false;
Scalar color = Scalar(0, 0, 255);

double prepareDoubleValue(int min, int max, int ratio) {
	double val = (double)min / max;
	return val * ratio;
}

double guardRange(double value) {
	if (value == 0.0) value += 0.0001;
	else if (value == 1.0) value -= 0.0001;
	return value;
}


bool pointInsideImage(Point2f point) {
	if (point.x < 0 || point.x > 640) return false;
	if (point.y < 0 || point.y > 480) return false;
	return true;
}

bool isPointMoving(Point2f p1, Point2f p2) {
	return (abs(p1.x - p2.x) >= 2 || 
		abs(p1.y - p2.y) >= 2);
}

void ValueChange(int, void*) {
	changed = true;
}

void draw(Mat frame, Point2f prevPoint, Point2f nextPoint) {
	line(frame, nextPoint, prevPoint, color, 1, 1, 0);
	circle(frame, nextPoint, 2, color, 1, 1, 0);
}


void cameraOpticalFlow() {
	VideoCapture cap(0);

	namedWindow("settings", 1);

	createTrackbar("maxCorners", "settings", &maxCorners, 200, ValueChange);
	createTrackbar("qualityLevel", "settings", &qualityLevel, 100);
	createTrackbar("minDistance", "settings", &minDistance, 20);

	double camWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	double camHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

	Mat frame, grayFrames, prevGrayFrame;
	Mat opticalFlow = Mat(camWidth, camHeight, CV_32FC3);

	vector<Point2f> nextPoints;
	vector<Point2f> prevPoints;

	vector<uchar> status;
	vector<float> err;

	bool needToInit = true;

	namedWindow(rawWindow, WINDOW_AUTOSIZE);

	while (true) {

		cap >> frame;
		cvtColor(frame, grayFrames, COLOR_BGR2GRAY);

		double newQuality = guardRange(prepareDoubleValue(qualityLevel, 100, 1));


		if (needToInit) {
			goodFeaturesToTrack(grayFrames, nextPoints, maxCorners, newQuality, minDistance, Mat());
			needToInit = false;
		}
		else if (!prevPoints.empty()) {
			calcOpticalFlowPyrLK(prevGrayFrame, grayFrames, prevPoints, nextPoints, status, err);

			for (int i = 0; i < prevPoints.size(); i++) {
				draw(frame, prevPoints[i], nextPoints[i]);
				draw(opticalFlow, prevPoints[i], nextPoints[i]);

				if (!pointInsideImage(nextPoints[i]) || !isPointMoving(nextPoints[i], prevPoints[i])) {
					nextPoints.erase(nextPoints.begin() + i);
					prevPoints.erase(prevPoints.begin() + i);
					//cout << "Point " << i << " deleted!" << endl;
				}

			}

		}

		if (nextPoints.size() < 10 || changed == true) {
			goodFeaturesToTrack(grayFrames, nextPoints, maxCorners, newQuality, minDistance, Mat());
			changed = false;
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

	frame.release();
	grayFrames.release();
	prevGrayFrame.release();
	destroyAllWindows();
}


void videoOpticalFlow() {
	VideoCapture cap("bike.avi");

	namedWindow("settings", 1);

	createTrackbar("maxCorners", "settings", &maxCorners, 200, ValueChange);
	createTrackbar("qualityLevel", "settings", &qualityLevel, 100);
	createTrackbar("minDistance", "settings", &minDistance, 20);

	double camWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	double camHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

	Mat frame, grayFrames, prevGrayFrame;
	Mat opticalFlow = Mat(camWidth, camHeight, CV_32FC3);

	vector<Point2f> nextPoints;
	vector<Point2f> prevPoints;

	vector<uchar> status;
	vector<float> err;

	bool needToInit = true;

	namedWindow(rawWindow, WINDOW_AUTOSIZE);

	while (cap.read(frame) && waitKey(20) != 27) {

		cvtColor(frame, grayFrames, COLOR_BGR2GRAY);
		double newQuality = guardRange(prepareDoubleValue(qualityLevel, 100, 1));

		if (needToInit) {
			goodFeaturesToTrack(grayFrames, nextPoints, maxCorners, newQuality, minDistance, Mat());
			needToInit = false;
		}
		else if (!prevPoints.empty()) {
			calcOpticalFlowPyrLK(prevGrayFrame, grayFrames, prevPoints, nextPoints, status, err);

			for (int i = 0; i < prevPoints.size(); i++) {
				
				draw(frame, prevPoints[i], nextPoints[i]);
				draw(opticalFlow, prevPoints[i], nextPoints[i]);

				if (!isPointMoving(nextPoints[i], prevPoints[i])) {
					nextPoints.erase(nextPoints.begin() + i);
					prevPoints.erase(prevPoints.begin() + i);
				}
			}
		}

		if (nextPoints.size() < 10 || changed == true) {
			goodFeaturesToTrack(grayFrames, nextPoints, maxCorners, newQuality, minDistance, Mat());
			changed = false;
		}


		swap(prevPoints, nextPoints);
		nextPoints.clear();
		grayFrames.copyTo(prevGrayFrame);

		imshow(rawWindow, frame);
		imshow(opticalFlowWindow, opticalFlow);
	}

	frame.release();
	grayFrames.release();
	prevGrayFrame.release();
	destroyAllWindows();
}

void printMenu() {
	cout << "MENU\n";
	cout << "[1] OpticalFlow Camera\n";
	cout << "[2] OpticalFlow Video\n";
	cout << "[ESC] To Exit\n";
}

int main() {
	char choice;
	do {
		printMenu();
		choice = _getch();
		switch (choice)
		{
		case '1':
			cameraOpticalFlow();
			break;
		case '2':
			videoOpticalFlow();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}
