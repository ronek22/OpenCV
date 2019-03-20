#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int negative() {
	VideoCapture cap;
	Mat original;
	Mat grey;

	cap.open(0);

	if (!cap.isOpened()) {
		cout << "Error opening camera";
		return -1;
	}

	namedWindow("Camera", WINDOW_AUTOSIZE);

	while (waitKey(10) != '\33') {
		cap >> original;
		cout << cap.get(CAP_PROP_POS_MSEC) << endl;
		cvtColor(original, grey, COLOR_BGR2GRAY);
		imshow("Camera", 255 - grey);
	}

	cap.release();
	destroyAllWindows();
}

int SaveFramesFromCamera() {
	VideoCapture cap;
	Mat frame;

	int frames;
	int count = 0;
	int timeoffset;

	cap.open(0);
	if (!cap.isOpened()) {
		cout << "Error opening camera";
		return -1;
	}

	namedWindow("Camera", WINDOW_AUTOSIZE);

	cout << "How many frames do you want to save? ";
	cin >> frames;

	while (count++ < frames) {
		cap >> frame;
		string path = "frames/" + to_string(count) + ".jpg";
		imwrite(path, frame);
		waitKey(10);
	}

	cap.release();
	destroyAllWindows();
}


void printMenu() {
	cout << "MENU\n";
	cout << "[1] Negative\n";
	cout << "[2] Save frames from camera\n";
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
			negative();
			break;
		case '2':
			SaveFramesFromCamera();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}