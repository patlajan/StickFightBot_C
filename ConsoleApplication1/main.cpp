#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <fstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include "rl.h"

using namespace std;
using namespace cv;

HWND hwnd;
int counter = 0;
double height_val = 0.3;
double ceiling_val = 0.6;

Scalar bad_color = Scalar(255, 255, 255);
Scalar good_color = Scalar(55, 118, 244);

double crop_y;
double crop_height;
cv::Rect myROI;

Mat processImg(Mat img) {
	int thresh = 100;
	Mat img2;
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat cropImg = img(myROI);

	inRange(cropImg, cv::Scalar(250, 250, 250), cv::Scalar(255, 255, 255), img2);

	/// Detect edges using canny
	Canny(img2, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<Point> contours_poly;
	vector<Rect> badGuys(3);
	vector<Point2f> badGuysCenters(3);
	/// Draw contours
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//if distance to previous less than 25px, then it's the same guy, so merge them.
		approxPolyDP(Mat(contours[i]), contours_poly, 3, true);
		drawContours(drawing, contours, i, bad_color, 2, 8, hierarchy, 0, Point());
		Rect boundRect = boundingRect(Mat(contours_poly));
		if (boundRect.x > 0 && boundRect.y > 0
			&& boundRect.height > 40 && boundRect.height < 75
			&& boundRect.width >= 20 && boundRect.width < 65
			&& boundRect.width * boundRect.height > 1000) { //do a better check for 'isBad'
															//rectangle(drawing, boundRect.tl(), boundRect.br(), bad_color, 2, 8, 0);
			rectangle(img, Point(boundRect.x, boundRect.y + crop_y), Point(boundRect.x + boundRect.width, boundRect.y + crop_y + boundRect.height), bad_color, 2, 8, 0);
			badGuys.push_back(boundRect);
			/*	Moments m = moments(contours[i], false);
			badGuysCenters.push_back(Point2f(m.m10 / m.m00, m.m01 / m.m00));
			circle(drawing, Point2f(m.m10 / m.m00, m.m01 / m.m00), 5, good_color, 5);*/
		}
		else { //do a better check for 'isBad'
			   //rectangle(drawing, boundRect.tl(), boundRect.br(), good_color, 2, 8, 0);
			rectangle(img, Point(boundRect.x, boundRect.y + crop_y), Point(boundRect.x + boundRect.width, boundRect.y + crop_y + boundRect.height), good_color, 2, 8, 0);
			badGuys.push_back(boundRect);
			/*Moments m = moments(contours[i], false);
			badGuysCenters.push_back(Point2f(m.m10 / m.m00, m.m01 / m.m00));
			circle(drawing, Point2f(m.m10 / m.m00, m.m01 / m.m00), 5, good_color, 5);*/
		}
	}

	inRange(cropImg, cv::Scalar(51, 114, 240), cv::Scalar(55, 118, 244), img2);
	/// Detect edges using canny
	Canny(img2, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	Rect goodGuy;
	for (int i = 0; i< contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly, 3, true);
		goodGuy = boundingRect(Mat(contours_poly));
		rectangle(drawing, goodGuy.tl(), goodGuy.br(), good_color, 2, 8, 0);

		rectangle(img, Point(goodGuy.x, goodGuy.y + crop_y), Point(goodGuy.x + goodGuy.width, goodGuy.y + crop_y + goodGuy.height), good_color, 2, 8, 0);
		drawContours(drawing, contours, i, good_color, 2, 8, hierarchy, 0, Point());
	}

	int minDist = 1000000;
	Rect minBadGuy;
	for (int i = 0; i < badGuys.size(); i++) {
		//hit the closest if close enough
		int d = abs(goodGuy.x + goodGuy.width / 2 - badGuys[i].x - badGuys[i].width / 2);

		if (d > 0 && d < minDist) {
			minDist = d;
			minBadGuy = badGuys[i];
		}
	}

	cvtColor(img, img, CV_RGBA2RGB);
	Mat res(Size(img.cols, drawing.rows + img.rows), CV_8UC3);
	Mat roi1 = res(Rect(0, 0, img.cols, img.rows));
	Mat roi2 = res(Rect(0, img.rows, drawing.cols, drawing.rows));
	int a = img.channels();
	img.copyTo(roi1);
	drawing.copyTo(roi2);

	//imwrite("C:\\Users\\Elian\\Desktop\\asd\\asd_" + to_string(counter++) + ".jpg", res);

	imshow("Orig", res);

	return drawing;
}

HDC hwindowDC, hwindowCompatibleDC;

int height, width, srcheight, srcwidth;
HBITMAP hbwindow;
Mat src;
BITMAPINFOHEADER bi;

extern "C" __declspec(dllexport) void destroy() {
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);
}

extern "C" __declspec(dllexport) void init() {
	hwnd = FindWindow(L"Qt5QWindowIcon", L"KOPLAYER 1.4.1055");
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	startWindowThread();
	namedWindow("Orig", WINDOW_AUTOSIZE);


	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom;  //change this to whatever size you want to resize to
	width = windowsize.right;
	crop_y = ceiling_val * height;
	crop_height = height_val * height;
	myROI = cv::Rect(0, crop_y, width, crop_height);

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
}


extern "C" __declspec(dllexport) void getNext() {
	SelectObject(hwindowCompatibleDC, hbwindow);
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, width, height, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

	Mat res = processImg(src);
	waitKey(1);

	//return res;
}