#include<opencv2/highgui/highgui.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int main() {
	// Utworzenie macierzy przechowuj¹cej obraz (o dowolnych, skoñczonych wymiarach)
	Mat image;
	// Inicjalizacja przechwytywania, VideoCapture klasa umozliwiajaca przechwytywanie obrazow z plikow video lub kamer
	VideoCapture cap;

	// Otwieranie kamery
	cap.open(0);

	if (!cap.isOpened()) {
		cout << "Wystapily problemy z otwarciem kamery";
		return -1;
	}

	namedWindow("window", WINDOW_AUTOSIZE);

	double dWidth = cap.set(CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CAP_PROP_FRAME_HEIGHT, 480);

	while (waitKey(15) != '\33') {
		cap >> image;
		// imshow potrzebuje regularnego wywolywanie waitKey by pozostac w petli
		// if you don't call waitKey, HighGui cannot process windows events like redraw, resizing, input event etc. So just call it, even with a 1ms delay 
		imshow("window", image);
	}

	return 0;



}