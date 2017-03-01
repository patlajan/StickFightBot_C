#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;

	/// Read the image
	src = imread("C:\\Users\\E\\Desktop\\asd\\asd.jpg", 1);

	if (!src.data)
	{
		return -1;
	}

	inRange(src, cv::Scalar(220, 220, 220), cv::Scalar(255, 255, 255), src_gray);

	/// Reduce the noise so we avoid false circle detection
	GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);

	std::vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, 20, 200, 100, 8, 16);

	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/// Show your results
	namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	imshow("Hough Circle Transform Demo", src);

	namedWindow("Hough Circle Transform", CV_WINDOW_AUTOSIZE);
	imshow("Hough Circle Transform", src_gray);

	waitKey(0);
	return 0;
}