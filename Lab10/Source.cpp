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
int scaleFactor = 3;
int minNeighbours = 3;
int minSize = 10;
int k, choose;

String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";

VideoCapture cap;
Mat frame, greyFrame, firstFace;
CascadeClassifier face_cascade, eyes_cascade;
Rect first_face_area;
vector<Rect> faces, eyes;



int findFaces() {
	cap.open(0);
	
	namedWindow("settings", 1);
	createTrackbar("scaleFactor", "settings", &scaleFactor, 100);
	createTrackbar("minNeighbours", "settings", &minNeighbours, 100);
	createTrackbar("minSize", "settings", &minSize, 100);

	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

	while (true) {
		cap >> frame;
		
		cvtColor(frame, greyFrame, COLOR_BGR2GRAY);
		equalizeHist(greyFrame, greyFrame);

		face_cascade.detectMultiScale(greyFrame, faces);

		for (int i = 0; i < faces.size(); i++) {
			Rect face_area = faces[i];
			Mat faceRect = greyFrame(face_area);

			eyes_cascade.detectMultiScale(faceRect, eyes, minNeighbours = 2);

			Point left_side(face_area.x, face_area.y + face_area.height / 2);
			Rect eyes_area(left_side - Point(0, face_area.height / 3), Size(face_area.width, face_area.height / 3));
			
			k = waitKey(1);
			if (k != -1) {
				choose = k;
			}

			switch (choose) {
			case 49:
				GaussianBlur(frame(face_area), frame(face_area), Size(15, 15), 20);
				break;
			case 50:
				rectangle(frame, eyes_area, Scalar(0, 0, 0), -1);
				break;
			case 51:
				if (i == 0) {
					first_face_area = face_area;
					firstFace = frame(first_face_area).clone();
				}
				else {
					resize(firstFace, firstFace, face_area.size());
					firstFace.copyTo(frame(face_area));
				}
				break;
			case 27:
				cout << "EXIT" << endl;
				destroyAllWindows();
				return 0;
			};
		}

		if (!frame.empty())
			imshow("Image", frame);
	}

	destroyAllWindows();

}

int main() {
	findFaces();

	return(0);
}