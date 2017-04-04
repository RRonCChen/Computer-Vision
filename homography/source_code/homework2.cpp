#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <windows.h> 
#include "opencv/opencv2/opencv.hpp"
#include <fstream>
#include <vector>

#ifdef ___WIN_X64_LIB___
#pragma comment(lib, "../../lib/x64/opencv_world320.lib")
#endif

#ifdef ___WIN_X86_LIB___
#pragma comment(lib, "../../lib/x86/opencv_world320.lib")
#endif

using namespace cv;
using namespace std;

vector<Point2f> img1_points;
vector<Point2f> img2_points;
vector<Point2f> img2_repair;
vector<Point2f> img_final;
Mat img1 = imread("1.jpg");
Mat img2 = imread("2.jpg");


void repair_img2() {
	//img1翴
	img1_points.push_back(Point2f(947, 505));
	img1_points.push_back(Point2f(939, 667));
	img1_points.push_back(Point2f(1555, 695));
	img1_points.push_back(Point2f(1555, 553));
	//img2翴
	img2_points.push_back(Point2f(987, 669));
	img2_points.push_back(Point2f(979, 823));
	img2_points.push_back(Point2f(1521, 893));
	img2_points.push_back(Point2f(1519, 759));
	//img1 to img2  H
	Mat H = findHomography(img1_points, img2_points);
	
	//璶干絛瞅
	Mat img2_black_white = imread("2_black_white.jpg");
	cout << "cols : " << img2_black_white.cols << endl;
	cout << "rows : " << img2_black_white.rows << endl;
	
	for (int x = 0; x < img2_black_white.cols; x++) {
		for (int y = 0; y < img2_black_white.rows; y++) {
			double b = img2_black_white.at<Vec3b>(y, x)[0];
			if (b != 0) {
				Mat point1 = (Mat_<double>(3, 1) << x, y, 1); //img2_black_whiteフ︹场だ
				Mat point2 = H.inv()*point1;
				
				//传衡img1翴
				double point_img1_x = point2.at<double>(0, 0) / point2.at<double>(0, 2);
				double point_img1_y = point2.at<double>(0, 1) / point2.at<double>(0, 2);
				
				//рimg1肅︹干img2
				img2.at<Vec3b>(y, x)[0] = img1.at<Vec3b>(int(point_img1_y), int(point_img1_x))[0];
				img2.at<Vec3b>(y, x)[1] = img1.at<Vec3b>(int(point_img1_y), int(point_img1_x))[1];
				img2.at<Vec3b>(y, x)[2] = img1.at<Vec3b>(int(point_img1_y), int(point_img1_x))[2];
			}
		}	
	}

}

void img2_homography() {
	img2_repair.push_back(Point2f(489,333));
	img2_repair.push_back(Point2f(477, 1753));
	img2_repair.push_back(Point2f(1937, 1665));
	img2_repair.push_back(Point2f(1869, 617));

	img_final.push_back(Point2f(0, 0));
	img_final.push_back(Point2f(0, 300));
	img_final.push_back(Point2f(400, 300));
	img_final.push_back(Point2f(400, 0));

	Mat H = findHomography(img2_repair, img_final);

	Mat result = Mat(300, 400, CV_8UC3);

	Mat img2_repair = imread("repair_img2.jpg");

	
	
	for (int x = 0; x < result.cols; x++) {
		for (int y = 0; y < result.rows; y++) {
			Mat point1 = (Mat_<double>(3, 1) << x, y, 1); 
			Mat point2 = H.inv()*point1;

			//传衡img2_repair翴
			double point_img2_repair_x = point2.at<double>(0, 0) / point2.at<double>(0, 2);
			double point_img2_repair_y = point2.at<double>(0, 1) / point2.at<double>(0, 2);
									
			//рimg2_repair肅︹干result
			result.at<Vec3b>(y, x)[0] = img2_repair.at<Vec3b>(int(point_img2_repair_y), int(point_img2_repair_x))[0];
			result.at<Vec3b>(y, x)[1] = img2_repair.at<Vec3b>(int(point_img2_repair_y), int(point_img2_repair_x))[1];
			result.at<Vec3b>(y, x)[2] = img2_repair.at<Vec3b>(int(point_img2_repair_y), int(point_img2_repair_x))[2];
						
		}	
	}
	//陪ボ纗挡狦
	namedWindow("Result", 0);
	imshow("Result", result);
	imwrite("result.jpg", result);
}
int main()
{
	
	repair_img2();
	imwrite("repair_img2.jpg", img2);
	img2_homography();

	waitKey(0);
	system("pause");
	return 0;
}