#include "TrackObject.h"



TrackObject::TrackObject(vector<Point> _contour)
{
	contour = _contour;
	calcCenter();
}

void TrackObject::calcCenter() {
	Moments oMoments = moments(contour);
	double M01 = oMoments.m01;
	double M10 = oMoments.m10;
	double area = oMoments.m00;

	center = Point(M10 / area, M01 / area);
}

void TrackObject::draw(Mat frame) {
	circle(frame, center, 43, Scalar(0, 0, 255), 3);
	string str = to_string(center.x).append(", ").append(to_string(center.y));
	putText(frame, str, center, FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255, 255));
}


TrackObject::~TrackObject()
{
}
