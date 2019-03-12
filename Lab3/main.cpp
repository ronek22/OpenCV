#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int moveDetectionVideo() {
	VideoCapture stream("robot_no_loop.avi");


	if (!stream.isOpened()) {
		cout << "Failed to open video capture" << endl;
		return -1;
	}

	Mat firstFrame, secondFrame, result;
	Mat firstFrame2, secondFrame2, result2;

	stream.read(firstFrame2);
	cvtColor(firstFrame2, firstFrame2, COLOR_RGB2GRAY);

	while (true) {
		stream >> firstFrame;
		stream >> secondFrame;

		if (firstFrame.empty() || secondFrame.empty()) {
			break;
		}

		imshow("cam1", secondFrame);

		stream.read(secondFrame2);
		cvtColor(secondFrame2, secondFrame2, COLOR_RGB2GRAY);

		cvtColor(firstFrame, firstFrame, COLOR_RGB2GRAY);
		cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);
		absdiff(firstFrame, secondFrame, result); // aktualna i poprzednia
		absdiff(firstFrame2, secondFrame2, result2); // pierwsza klatka - aktualna
		threshold(result, result, 100, 255, THRESH_BINARY);
		threshold(result2, result2, 100, 255, THRESH_BINARY);

		imshow("cam2", result);
		imshow("cam3", result2);

		if (waitKey(30) == 27)
			break;

	}

	stream.release();
	destroyAllWindows();
}

int moveDetectionCamera() {
	VideoCapture cap(0);


	if (!cap.isOpened()) {
		cout << "Failed to open video capture" << endl;
		return -1;
	}

	Mat firstFrame, secondFrame, result;
	Mat firstFrame2, secondFrame2, result2;

	cap.read(firstFrame2);
	cvtColor(firstFrame2, firstFrame2, COLOR_RGB2GRAY);

	while (true) {
		cap >> firstFrame;
		cap >> secondFrame;

		if (firstFrame.empty() || secondFrame.empty()) {
			break;
		}

		imshow("cam1", secondFrame);

		cap.read(secondFrame2);
		cvtColor(secondFrame2, secondFrame2, COLOR_RGB2GRAY);



		cvtColor(firstFrame, firstFrame, COLOR_RGB2GRAY);
		cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);
		absdiff(firstFrame, secondFrame, result);
		absdiff(firstFrame2, secondFrame2, result2);
		threshold(result, result, 100, 255, THRESH_BINARY);
		threshold(result2, result2, 100, 255, THRESH_BINARY);

		imshow("cam2", result);
		imshow("cam3", result2);

		if (waitKey(30) == 27)
			break;

	}

	cap.release();
	destroyAllWindows();
}

int addLogoToCamera() {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Failed to open video capture" << endl;
		return -1;
	}

	Mat image = imread("logo.bmp");
	Mat frame;
	int size = 70;
	resize(image, image, Size(size, size));


	while (waitKey(15) != '\33') {
		cap >> frame;

		Mat insetImage(frame, Rect(0, 0, 70, 70));
		image.copyTo(insetImage);
		imshow("frame", frame);
	}

	cap.release();
	destroyAllWindows();

	return 0;
}


int addLogoToVideo() {
	VideoCapture stream("robot_no_loop.avi");
	if (!stream.isOpened()) {
		cout << "Failed to open video capture" << endl;
		return -1;
	}

	Mat image = imread("logo.bmp");
	Mat frame;
	int size = 70;
	resize(image, image, Size(size, size));


	while (waitKey(15) != '\33') {
		stream >> frame;

		Mat insetImage(frame, Rect(0, 0, 70, 70));
		image.copyTo(insetImage);
		imshow("frame", frame);
	}

	stream.release();
	destroyAllWindows();

	return 0;
}
void printMenu() {
	cout << "MENU\n";
	cout << "[1] Move detection camera\n";
	cout << "[2] Move detection video.\n";
	cout << "[3] Add logo to camera\n";
	cout << "[4] Add logo to video\n";
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
			moveDetectionCamera();
			break;
		case '2':
			moveDetectionVideo();
			break;
		case '3':
			addLogoToCamera();
			break;
		case '4':
			addLogoToVideo();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}