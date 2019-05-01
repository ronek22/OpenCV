#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/videoio.hpp>
#include<opencv2/video.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<conio.h>
#include<ctime>
using namespace cv;
using namespace std;


int MOGCamera() {
	VideoCapture cap(0);
	Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();
	Mat orig, background, foreground;
	Mat morphKernel = getStructuringElement(MORPH_RECT, Size(6, 6));
	
	namedWindow("Background", 1);
	namedWindow("Motion", 1);
	namedWindow("settings", 1);

	int subtractor_history = 30;
	int subtractor_mixtures = 5;
	int shadows = 1;

	createTrackbar("history", "settings", &subtractor_history, 50);
	createTrackbar("mixtures", "settings", &subtractor_mixtures, 6);
	createTrackbar("shadows", "settings", &shadows, 1);

	while (waitKey(20) != 27) {
		cap.read(orig);

		pMOG2->apply(orig, foreground);

		pMOG2->setHistory(subtractor_history);
		pMOG2->setNMixtures(subtractor_mixtures);
		pMOG2->setDetectShadows(shadows);

		pMOG2->getBackgroundImage(background);

		// advanced erosion and dilation
		morphologyEx(foreground, foreground, MORPH_OPEN, morphKernel);

		vector<vector<Point>> contours;
		findContours(foreground, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < contours.size(); i++) {
			drawContours(orig, contours, i, Scalar(0, 0, 255), 2);
		}

		imshow("Background", background);
		imshow("Motion", orig);
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}

int MOGVideo() {
	VideoCapture cap("bike.avi");
	if (!cap.isOpened()) {
		cout << "Failed to open video streamture" << endl;
		return -1;
	}

	Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();
	Mat orig, background, foreground;
	Mat morphKernel = getStructuringElement(MORPH_RECT, Size(6, 6));

	namedWindow("Background", 1);
	namedWindow("Motion", 1);
	namedWindow("settings", 1);

	int subtractor_history = 30;
	int subtractor_mixtures = 5;
	int shadows = 1;

	createTrackbar("history", "settings", &subtractor_history, 50);
	createTrackbar("mixtures", "settings", &subtractor_mixtures, 6);
	createTrackbar("shadows", "settings", &shadows, 1);

	while (cap.read(orig) && waitKey(20) != 27) {

		pMOG2->apply(orig, foreground);

		pMOG2->setHistory(subtractor_history);
		pMOG2->setNMixtures(subtractor_mixtures);
		pMOG2->setDetectShadows(shadows);

		pMOG2->getBackgroundImage(background);

		// advanced erosion and dilation
		morphologyEx(foreground, foreground, MORPH_OPEN, morphKernel);

		vector<vector<Point>> contours;
		findContours(foreground, contours, RETR_LIST, CHAIN_APPROX_NONE);

		for (int i = 0; i < contours.size(); i++) {
			drawContours(orig, contours, i, Scalar(0, 0, 255), 2);
		}

		imshow("Background", background);
		imshow("Motion", orig);
	}

	cap.release();
	cv::destroyAllWindows();

	return 0;
}

int HoughCamera() {
	VideoCapture cap(0);

	int line_length = 50;
	int threshold = 50;
	int line_gap = 10;


	namedWindow("settings", 1);
	createTrackbar("line length", "settings", &line_length, 100);
	createTrackbar("line gap", "settings", &line_gap, 100);
	createTrackbar("threshold", "settings", &threshold, 100);

	Mat frame, edge;

	if (!cap.isOpened()) {
		cout << "Error during opening the video file.";
		return -1;
	}

	while (waitKey(20) != 27) {
		cap.read(frame);

		Canny(frame, edge, 50, 200, 3);

		// Probabilistic Hough Line Transform
		vector<Vec4i> lines;
		HoughLinesP(edge, lines, 1, CV_PI / 180, threshold, line_length, line_gap);

		for (int i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3);
		}

		imshow("window", frame);
		imshow("canny", edge);
	}

	cap.release();
	cv::destroyAllWindows();
}






void printMenu() {
	cout << "MENU\n";
	cout << "[1] MOG Camera\n";
	cout << "[2] MOG Video\n";
	cout << "[3] Hough Camera\n";
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
			MOGCamera();
			break;
		case '2':
			MOGVideo();
			break;
		case '3':
			HoughCamera();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}