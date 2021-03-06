#include<opencv2/highgui/highgui.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int getOdd(int value) {
	if (value % 2 == 0) 
		return value + 1;
	return value;
}


int firstExercise() {
	VideoCapture cap(0);

	Mat frame, gauss, sobel_x, sobel_y, detected_edges;
	Mat sobel_x_bin, sobel_y_bin;
	Mat gradient, angle, gradient_bin, gradient_colored;

	if (!cap.isOpened()) {
		cout << "Cannot open camera";
		return -1;
	}

	
	int threshold_bottom = 50;
	int threshold_up = 150;
	int gauss_size = 3;
	int sobel_mask = 3;
	int prog_bin = 20;
	int prog_bin_max = 255;

	namedWindow("settings", 1);
	createTrackbar("threshold_bottom", "settings", &threshold_bottom, 100);
	createTrackbar("threshold_top", "settings", &threshold_up, 250);
	createTrackbar("gauss", "settings", &gauss_size, 250);
	createTrackbar("sobel_mask", "settings", &sobel_mask, 9);

	while (waitKey(30) != 27) {
		cap.read(frame);

		gauss_size = getOdd(gauss_size);
		sobel_mask = getOdd(gauss_size);
		
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		GaussianBlur(frame, gauss, Size(gauss_size, gauss_size), 2.0);
		Canny(gauss, detected_edges, threshold_bottom, threshold_up, 3);

		Sobel(gauss, sobel_x, CV_32F, 1, 0, sobel_mask);
		threshold(sobel_x, sobel_x_bin, prog_bin, prog_bin_max, THRESH_BINARY);
		Sobel(gauss, sobel_y, CV_32F, 0, 1, sobel_mask);
		threshold(sobel_y, sobel_y_bin, prog_bin, prog_bin_max, THRESH_BINARY);
		
		cartToPolar(sobel_x, sobel_y, gradient, angle, true);
		threshold(gradient, gradient_bin, prog_bin, prog_bin_max, THRESH_BINARY);

		
		gradient.copyTo(gradient_colored);
		cvtColor(gradient_colored, gradient_colored, COLOR_GRAY2BGR);
		gradient_colored.convertTo(gradient_colored, CV_8UC3, 255);
		float angle_value;

		Vec3b red = Vec3b(0, 0, 255);
		Vec3b green = Vec3b(0, 255, 0);
		Vec3b blue = Vec3b(255, 0, 0);
		Vec3b white = Vec3b(255, 255, 255);
		Vec3b black = Vec3b(0, 0, 0);

		for (int i = 0; i < gradient.rows; i++) {
			for (int j = 0; j < gradient.cols; j++) {
				angle_value = angle.at<float>(i, j);
				Vec3b *color = &gradient_colored.at<Vec3b>(i, j);
				float *color_bin = &gradient_bin.at<float>(i, j);

				if (*color_bin > 0) {
					if (angle_value > 45 && angle_value <= 135)
						* color = white;
					if (angle_value > 135 && angle_value <= 255)
						* color = blue;
					if (angle_value > 255 && angle_value <= 315)
						* color = green;
					if ((angle_value > 315 && angle_value <= 360) || (angle_value > 0 && angle_value <= 45))
						* color = red;
				}
				else {
					* color = black;
				}

			
			}
		}

		imshow("gauss", gauss);
		imshow("sobel x", sobel_x_bin);
		imshow("sobel y", sobel_y_bin);
		imshow("canny", detected_edges);
		imshow("gradient magnitude binary", gradient_bin);
		imshow("gradient colored", gradient_colored);

	}

	gauss.release();
	sobel_x.release();
	sobel_y.release();
	detected_edges.release();
	cap.release();
	cv::destroyAllWindows();

	return 0;
}



int main() {
	firstExercise();
	return(0);
}