#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int moveDetection() {
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
		absdiff(firstFrame, secondFrame, result);
		absdiff(firstFrame2, secondFrame2, result2);
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



void printMenu() {
	cout << "MENU\n";
	cout << "[1] Move detection camera\n";
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
			moveDetection();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}