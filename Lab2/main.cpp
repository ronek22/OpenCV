#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int main() {
	Mat image = Mat::zeros(400, 400, CV_8UC3);

	line(image, Point(15, 20), Point(70, 50), Scalar(110, 220, 0), 2, LINE_AA);
	circle(image, Point(200, 200), 32.0, Scalar(0, 0, 255), 1, LINE_AA);
	rectangle(image, Point(15, 20), Point(70, 50), Scalar(0, 55, 255), +1, 4);
	putText(image, "Hi all...", Point(50, 100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 200, 200), 4, LINE_AA);
	imshow("Image", image);

	waitKey(0);
	return(0);
}