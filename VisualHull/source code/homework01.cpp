// homework01.cpp : Defines the entry point for the console application.
//

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



void loadImg(String filename);
bool isInside(Mat &img, Mat &xWorld, Mat &extrinsic, Mat &intrinsic);
void writeXYZ(String filename);

struct point {
	int x;
	int y;
	int z;
};

std::vector<point> volexs;

int main()
{
	char filename[50];
	while (1) {
		std::cout << "請輸入圖片資料夾名稱 : ";
		scanf("%s", &filename);

		std::fstream file;
		String exitFile = filename;
		exitFile += "/001.bmp";
		file.open(exitFile, std::ios::in);

		if (file) {
			std::cout << "處理中....." << std::endl;
			loadImg(filename);
			break;
		}
		else {
			std::cout << "目標資料夾不存在....." << std::endl;
		}
	}
	system("pause");
	return 0;
}

void loadImg(String filename) {
	Mat intrinsic = (Mat_<float>(3, 3) <<
		714.074036, 0.000000, 400.000000,
		0.000000, 714.073975, 300.000000,
		0.000000, 0.000000, 1.000000);

	Mat extrinsic_1 = (Mat_<float>(3, 4) <<
		0.866849, -0.493982, -0.067491, 5.680202,
		-0.033026, 0.078177, -0.996382, 40.192253,
		0.497476, 0.865950, 0.051456, 131.575211);

	Mat extrinsic_2 = (Mat_<float>(3, 4) <<
		0.691898, 0.718699, -0.068914, 3.033622,
		-0.018523, -0.077748, -0.996791, 40.538036,
		-0.721758, 0.690961, -0.040479, 132.814957);

	Mat extrinsic_3 = (Mat_<float>(3, 4) <<
		-0.708825, 0.705031, -0.022315, 2.710659,
		-0.023775, -0.055497, -0.998163, 40.025272,
		-0.704984, -0.707001, 0.056103, 131.650055);


	Mat extrinsic_4 = (Mat_<float>(3, 4) <<
		-0.990984, -0.131983, -0.023039, -1.359515,
		0.033893, -0.080598, -0.996156, 40.564919,
		0.129620, -0.987970, 0.084348, 126.794678);

	Mat extrinsic_5 = (Mat_<float>(3, 4) <<
		-0.071742, -0.997325, 0.014031, 10.313296,
		-0.040528, -0.011141, -0.999102, 40.912224,
		0.996600, -0.072247, -0.039619, 127.704018);

	Mat extrinsic_6 = (Mat_<float>(3, 4) <<
		-0.946351, -0.190357, 0.261120, -10.116337,
		-0.241168, 0.953881, -0.178661, 7.099833,
		-0.215071, -0.232055, -0.948628, 156.788635);

	Mat extrinsic_7 = (Mat_<float>(3, 4) <<
		-0.280008, 0.952858, 0.116867, -2.020520,
		-0.725914, -0.130496, -0.675269, 28.864723,
		-0.628193, -0.273920, 0.728245, 106.115578);

	Mat extrinsic_8 = (Mat_<float>(3, 4) <<
		0.897493, -0.360072, -0.254667, 8.695093,
		0.088773, 0.713100, -0.695396, 31.042011,
		0.432002, 0.601513, 0.671979, 125.422333);

	Mat img1 = cv::imread(filename + "/001.bmp",0);
	Mat img2 = cv::imread(filename + "/002.bmp", 0);
	Mat img3 = cv::imread(filename + "/003.bmp", 0);
	Mat img4 = cv::imread(filename + "/004.bmp", 0);
	Mat img5 = cv::imread(filename + "/005.bmp", 0);
	Mat img6 = cv::imread(filename + "/006.bmp", 0);
	Mat img7 = cv::imread(filename + "/007.bmp", 0);
	Mat img8 = cv::imread(filename + "/008.bmp", 0);

	
		for (int x = -50; x < 51; x++) {
			for (int y = -50; y < 51; y++) {
				for (int z = -10; z < 91; z++) {
					cv::Mat xWorld = (Mat_<float>(4, 1) << x, y, z, 1);
					bool flag1 = isInside(img1, xWorld, extrinsic_1, intrinsic);
					bool flag2 = isInside(img2, xWorld, extrinsic_2, intrinsic);
					bool flag3 = isInside(img3, xWorld, extrinsic_3, intrinsic);
					bool flag4 = isInside(img4, xWorld, extrinsic_4, intrinsic);
					bool flag5 = isInside(img5, xWorld, extrinsic_5, intrinsic);
					bool flag6 = isInside(img6, xWorld, extrinsic_6, intrinsic);
					bool flag7 = isInside(img7, xWorld, extrinsic_7, intrinsic);
					bool flag8 = isInside(img8, xWorld, extrinsic_8, intrinsic);

					if (flag1 == true && flag2 == true && flag3 == true && flag4 == true && flag5 == true && flag6 == true && flag7 == true && flag8 == true) {
						point volex;
						volex.x = x;
						volex.y = y;
						volex.z = z;
						volexs.push_back(volex);
					}
				}
			}
		}
		writeXYZ(filename);
	
}


bool isInside(Mat &img, Mat &xWorld,Mat &extrinsic,Mat &intrinsic){
	Mat xImg = Mat_<float>(3, 1);
	xImg = intrinsic * extrinsic * xWorld;	
	
	double pointX = xImg.at<float>(0, 0);
	double pointY = xImg.at<float>(1, 0);
	double pointZ = xImg.at<float>(2, 0);

	double pixelX = pointX / pointZ;
	double pixelY = pointY / pointZ;
	
	int height = img.rows;
	int width = img.cols;

	//超過邊界的判斷
	if (pixelX > width || pixelX < 0) {
		return false;
	}
	if (pixelY > height || pixelY < 0) {
		return false;
	}

	double intensity = img.at<uchar>(int(pixelY), int(pixelX));

	//判斷照片pixel
	if (intensity != 0) {	
		return true;	
	}

}

void writeXYZ(String filename) {
    std::fstream file;
	file.open(filename+".xyz",std::ios::out);
		
	for (int i = 0; i < volexs.size(); i++) {
		file << volexs[i].x << " " << volexs[i].y << " " << volexs[i].z << std::endl;
    }

	std::cout << filename  << "  finshed!!" << std::endl;
}



