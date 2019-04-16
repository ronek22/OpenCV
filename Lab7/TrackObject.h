#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;
#pragma once
class TrackObject
{
private:
	vector<Point> contour;
	Point center;

public:
	TrackObject(vector<Point> _contour);
	void calcCenter();
	void draw(Mat frame);


	~TrackObject();
};

