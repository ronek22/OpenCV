#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include<ctime>

using namespace cv;
using namespace std;


int firstExercise() {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error opening camera" << endl;
		return -1;
	}

	int dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	int dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	Mat imgLines = Mat::zeros(dHeight, dWidth, CV_8UC3);


	// green
	int hmin = 34;
	int hmax = 82;
	int smin = 65;
	int smax = 253;
	int vmin = 75;
	int vmax = 154;

	namedWindow("Control", 1);
	createTrackbar("H min", "Control", &hmin, 179);
	createTrackbar("H max", "Control", &hmax, 179);
	createTrackbar("S min", "Control", &smin, 255);
	createTrackbar("S max", "Control", &smax, 255);
	createTrackbar("V min", "Control", &vmin, 255);
	createTrackbar("V max", "Control", &vmax, 255);



	Mat frame_orig, frame_hsv, frame_threshold;
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	while (waitKey(30) != 27) {
		cap.read(frame_orig);
		
		cvtColor(frame_orig, frame_hsv, COLOR_BGR2HSV);
		inRange(frame_hsv, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), frame_threshold);

		
		erode(frame_threshold, frame_threshold, erodeElement);
		erode(frame_threshold, frame_threshold, erodeElement);
		dilate(frame_threshold, frame_threshold, dilateElement);
		dilate(frame_threshold, frame_threshold, dilateElement);


		vector<vector<Point>> contours;
		findContours(frame_threshold, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

		
		double largest_area = 0;
		int largest_contour = 0;
		if (contours.size() > 0) {
			for (int i = 0; i < contours.size(); i++) {
				double area = contourArea(contours[i], false);
				if (area > largest_area) {
					largest_area = area;
					largest_contour = i;
				}
			}

		Moments oMoments = moments(contours[largest_contour]);
		double M01 = oMoments.m01;
		double M10 = oMoments.m10;
		double area = oMoments.m00;

		int pos_x = M10 / area;
		int pos_y = M01 / area;

		circle(frame_orig, Point(pos_x, pos_y), 43, Scalar(0, 0, 255), 3);
		line(frame_orig, Point(pos_x - 75, pos_y), Point(pos_x - 10, pos_y), Scalar(0, 0, 255), 3);
		line(frame_orig, Point(pos_x + 10, pos_y), Point(pos_x + 75, pos_y), Scalar(0, 0, 255), 3);
		line(frame_orig, Point(pos_x, pos_y - 75), Point(pos_x, pos_y - 10), Scalar(0, 0, 255), 3);
		line(frame_orig, Point(pos_x, pos_y + 10), Point(pos_x, pos_y + 75), Scalar(0, 0, 255), 3);
		string str = to_string(pos_x).append(", ").append(to_string(pos_y));
		putText(frame_orig, str, Point(pos_x - 70, pos_y + 105), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));

		}




		imshow("Original", frame_orig);
		imshow("Threshold", frame_threshold);
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}

int secondExercise() {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Error opening camera" << endl;
		return -1;
	}


	clock_t current_time;
	int i = 0;
	int dWidth = cap.get(CAP_PROP_FRAME_WIDTH);
	int dHeight = cap.get(CAP_PROP_FRAME_HEIGHT);
	Mat imgLines = Mat::zeros(dHeight, dWidth, CV_8UC3);


	int hmin[3] = { 0, 34, 89 };
	int hmax[3] = { 41, 82, 147 };
	int smin[3] = { 116, 65,144 };
	int smax[3] = { 226,253,318 };
	int vmin[3] = { 171,75,45 };
	int vmax[3] = { 219,154,195 };


	Mat frame_orig, frame_hsv, frame_threshold;
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));

	clock_t last_recorded_time = clock() / CLOCKS_PER_SEC;
	while (waitKey(20) != 27) {
		cap.read(frame_orig);

		current_time = clock() / CLOCKS_PER_SEC;


		cvtColor(frame_orig, frame_hsv, COLOR_BGR2HSV);
		inRange(frame_hsv, Scalar(hmin[i], smin[i], vmin[i]), Scalar(hmax[i], smax[i], vmax[i]), frame_threshold);


		erode(frame_threshold, frame_threshold, erodeElement);
		erode(frame_threshold, frame_threshold, erodeElement);
		dilate(frame_threshold, frame_threshold, dilateElement);
		dilate(frame_threshold, frame_threshold, dilateElement);

		if (current_time - last_recorded_time >= 5)
		{
			putText(frame_orig, "ZMIANA!", Point(70, 70), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));
			if (i == 0)
				i = 1;
			else if (i == 1)
				i = 2;
			else if (i == 2)
				i = 0;
			last_recorded_time = current_time;
		}


		vector<vector<Point>> contours;
		findContours(frame_threshold, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);


		double largest_area = 0;
		int largest_contour = 0;
		if (contours.size() > 0) {
			for (int j = 0; j < contours.size(); j++) {
				double area = contourArea(contours[j], false);
				if (area > largest_area) {
					largest_area = area;
					largest_contour = j;
				}
			}

			Moments oMoments = moments(contours[largest_contour]);
			double M01 = oMoments.m01;
			double M10 = oMoments.m10;
			double area = oMoments.m00;

			int pos_x = M10 / area;
			int pos_y = M01 / area;

			circle(frame_orig, Point(pos_x, pos_y), 43, Scalar(0, 0, 255), 3);
			line(frame_orig, Point(pos_x - 75, pos_y), Point(pos_x - 10, pos_y), Scalar(0, 0, 255), 3);
			line(frame_orig, Point(pos_x + 10, pos_y), Point(pos_x + 75, pos_y), Scalar(0, 0, 255), 3);
			line(frame_orig, Point(pos_x, pos_y - 75), Point(pos_x, pos_y - 10), Scalar(0, 0, 255), 3);
			line(frame_orig, Point(pos_x, pos_y + 10), Point(pos_x, pos_y + 75), Scalar(0, 0, 255), 3);
			string str = to_string(pos_x).append(", ").append(to_string(pos_y));
			putText(frame_orig, str, Point(pos_x - 70, pos_y + 105), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));

		}

		imshow("Original", frame_orig);
		imshow("Threshold", frame_threshold);
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}




void printMenu() {
	cout << "MENU\n";
	cout << "[1] First exercise\n";
	cout << "[2] Second exercise\n";
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
			firstExercise();
			break;
		case '2':
			secondExercise();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}