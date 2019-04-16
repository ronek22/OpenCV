#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include<ctime>
#include "track_color.cpp"
#include "TrackObject.h"

using namespace cv;
using namespace std;


int firstExercise() {
	VideoCapture cap(0);

	clock_t current_time;
	int i = 2;

	Mat frame_orig, frame_hsv, frame_threshold;

	int dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	int dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	Mat imgLines = Mat::zeros(dHeight, dWidth, CV_8UC3);
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	int hmin[3] = { 0, 34, 89 };
	int hmax[3] = { 41, 82, 147 };
	int smin[3] = { 116, 65,144 };
	int smax[3] = { 226,253,318 };
	int vmin[3] = { 171,75,45 };
	int vmax[3] = { 219,154,195 };

	namedWindow("Control", 1);
	createTrackbar("Color", "Control", &i, 2);




	while (waitKey(20) != 27) {
		cap.read(frame_orig);


		cvtColor(frame_orig, frame_hsv, COLOR_BGR2HSV);
		inRange(frame_hsv, Scalar(hmin[i], smin[i], vmin[i]), Scalar(hmax[i], smax[i], vmax[i]), frame_threshold);


		erode(frame_threshold, frame_threshold, erodeElement);
		erode(frame_threshold, frame_threshold, erodeElement);
		dilate(frame_threshold, frame_threshold, dilateElement);
		dilate(frame_threshold, frame_threshold, dilateElement);

		vector<vector<Point>> contours;
		findContours(frame_threshold, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		vector<TrackObject> tracked_colors;

		for (vector<Point> contour : contours) {
			TrackObject object(contour);
			tracked_colors.push_back(object);
		}

		for (TrackObject object : tracked_colors) {
			object.draw(frame_orig);
		}

		imshow("Original", frame_orig);
	}

	cap.release();
	destroyAllWindows();



	return 0;
}



int main() {

	firstExercise();
	return(0);
}