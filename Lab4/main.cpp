#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;


int detectionCamera() {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Failed to open video capture" << endl;
		return -1;
	}

	Mat firstFrame, secondFrame, thirdFrame;
	Mat detectorFrame, result;

	cap >> firstFrame; 
	cvtColor(firstFrame, firstFrame, COLOR_BGR2GRAY);
	firstFrame.copyTo(detectorFrame);
	detectorFrame = detectorFrame - detectorFrame; // wyzerowanie macierzy

	int alpha_slider = 20;
	int threshold_slider = 60;
	
	namedWindow("settings", 1);
	createTrackbar("Alpha", "settings", &alpha_slider, 100);
	createTrackbar("threshold", "settings", &threshold_slider, 100);

	while (true) {
		cap >> secondFrame;
		cap >> thirdFrame;

		cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);
		cvtColor(thirdFrame, thirdFrame, COLOR_RGB2GRAY);
		absdiff(secondFrame, thirdFrame, result);
		threshold(result, result, threshold_slider, 255, THRESH_BINARY);

		double alpha = (double)alpha_slider / 100;
		detectorFrame = (1 - alpha) * detectorFrame + alpha * result;

		imshow("cam", detectorFrame);


		if (waitKey(30) == 27)
			break;

	}

	cap.release();
	firstFrame.release();
	secondFrame.release();
	thirdFrame.release();
	detectorFrame.release();
	result.release();
	destroyAllWindows();
}

int detectionVideo() {
	VideoCapture stream("bike.avi");
	if (!stream.isOpened()) {
		cout << "Failed to open video streamture" << endl;
		return -1;
	}

	Mat firstFrame, secondFrame, thirdFrame;
	Mat detectorFrame, result;

	int dWidth = stream.get(CAP_PROP_FRAME_WIDTH);
	int dHeight = stream.get(CAP_PROP_FRAME_HEIGHT);

	detectorFrame = Mat::zeros(dHeight, dWidth, CV_32F);

	int alpha_slider = 20;
	int threshold_slider = 60;

	namedWindow("settings", 1);
	createTrackbar("Alpha", "settings", &alpha_slider, 100);
	createTrackbar("threshold", "settings", &threshold_slider, 100);

	while (stream.read(secondFrame) && stream.read(thirdFrame)) {
		cvtColor(secondFrame, secondFrame, COLOR_RGB2GRAY);
		cvtColor(thirdFrame, thirdFrame, COLOR_RGB2GRAY);
		absdiff(secondFrame, thirdFrame, result);
		threshold(result, result, threshold_slider, 255, THRESH_BINARY);

		double alpha = (double)alpha_slider / 100;
		result.convertTo(result, CV_32F);
		accumulateWeighted(result, detectorFrame, alpha);

		imshow("cam", detectorFrame);

		if (waitKey(30) == 27)
			break;
	}

	stream.release();
	firstFrame.release();
	secondFrame.release();
	thirdFrame.release();
	detectorFrame.release();
	result.release();
	destroyAllWindows();
}

int SaveFramesFromCamera(int frames) {
	VideoCapture cap;
	Mat frame;

	int count = 0;
	int timeoffset;

	cap.open(0);
	if (!cap.isOpened()) {
		cout << "Error opening camera";
		return -1;
	}

	namedWindow("Camera", WINDOW_AUTOSIZE);

	while (count++ < frames) {
		cap >> frame;
		string path = "frames/" + to_string(count) + ".jpg";
		imwrite(path, frame);
		waitKey(10);
	}

	cap.release();
	destroyAllWindows();
}

int SaveFramesFromVideo() {
	VideoCapture cap("bike.avi");
	Mat frame;

	if (!cap.isOpened()) {
		cout << "Error opening camera";
		return -1;
	}

	namedWindow("Camera", WINDOW_AUTOSIZE);

	int count = 0;
	while (cap.read(frame)) {
		string path = "frames_video/" + to_string(count++) + ".jpg";
		imwrite(path, frame);
		waitKey(10);
	}

	cap.release();
	destroyAllWindows();
}

int saveCamera() {
	SaveFramesFromCamera(20);
	
	vector<String> fn;
	glob("./frames/*.jpg", fn, false);

	vector<Mat> images;
	size_t count = fn.size();

	for (size_t i = 0; i < count; i++) {
		images.push_back(imread(fn[i]));
	}

	Size S = images[0].size();
	VideoWriter outputVideo("out.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'),30, S, true);

	for (int i = 0; i < images.size(); i++) {
		outputVideo << images[i];
	}


	return 0;

}

int saveVideo() {
	SaveFramesFromVideo();

	vector<String> fn;
	glob("./frames_video/*.jpg", fn, false);

	vector<Mat> images;
	size_t count = fn.size();

	for (size_t i = 0; i < count; i++) {
		images.push_back(imread(fn[i]));
	}

	Size S = images[0].size();
	VideoWriter outputVideo("out.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), 30, S, true);

	for (int i = 0; i < images.size(); i++) {
		outputVideo << images[i];
	}


	return 0;

}

void printMenu() {
	cout << "MENU\n";
	cout << "[1] First exercise\n";
	cout << "[2] Second exercise\n";
	cout << "[3] Save video\n";
	cout << "[4] Save camera\n";
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
			detectionCamera();
			break;
		case '2':
			detectionVideo();
			break;
		case '3':
			saveVideo();
			break;
		case '4':
			saveCamera();
			break;
		default:
			break;
		}
	} while (choice != '\33');



	return(0);
}