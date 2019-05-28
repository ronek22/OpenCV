#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <ctype.h>
#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <conio.h>

using namespace cv;
using namespace std;

// Settings
int k, choose;
Mat frame1, frame2, grayFrame1, grayFrame2, diff, blurDiff, thresh, hull;
vector<vector<Point>> contours;
vector<Point> largestContour;
VideoCapture cap;
Rect bounding_rect;
int pos_x, pos_y;
bool turnOn = true;

void setLargestContour() {
	double largest_area = 0;
	if (contours.size() > 0) {
		for (int i = 0; i < contours.size(); i++) {
			double area = contourArea(contours[i], false);
			if (area > largest_area) {
				largest_area = area;
				largestContour = contours[i];
			}
		}
	}
}

void setCoords() {
	pos_x = bounding_rect.x;
	pos_y = bounding_rect.y;
}

void exercise() {
	cap.open(0);

	while (waitKey(20) != 27) {
		cap.read(frame1);
		cvtColor(frame1, grayFrame1, COLOR_BGR2GRAY);
		cap.read(frame2);
		cvtColor(frame2, grayFrame2, COLOR_BGR2GRAY);

		if (turnOn) {
			// STEP 2
			absdiff(grayFrame1, grayFrame2, diff);
			// STEP 3
			GaussianBlur(diff, blurDiff, Size(5, 5), 20);
			// STEP 4
			threshold(blurDiff, thresh, 20.0, 255.0, THRESH_BINARY);
			// STEP 5
			findContours(thresh, contours, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			setLargestContour();
			// STEP 6
			if (contours.size() > 0)
			{
				bounding_rect = boundingRect(largestContour);
				setCoords();
				convexHull(largestContour, hull);
				vector<vector<Point>> drawing = { hull };
				drawContours(frame2, drawing, -1, Scalar(0, 0, 255), 2);
				rectangle(frame2, bounding_rect, Scalar(255, 0, 0), 2);

				string str = to_string(pos_x).append(", ").append(to_string(pos_y));
				putText(frame2, str, Point(pos_x - 70, pos_y + 105), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));
			}
		}

		choose = waitKey(1);

		if (choose == 'p')
			while (waitKey(1) != 'p');

		if (choose == 'r') {
			if (turnOn) {
				cout << "TURN OFF\n";
				turnOn = false;
			}
			else {
				cout << "TURN ON\n";
				turnOn = true;
			}
		}

		imshow("Motion", frame2);
	}

	destroyAllWindows();

}

int main() {
	exercise();

	return(0);
}