#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <Calibrator.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace cv;

TEST(Calibrator, DetectsCheckerboard)
{
	Mat checkImg = imread("data/checkerboard1.png");
	Size checkSize = Size(8, 6);
	Mat corners;
	ASSERT_TRUE(checkImg.data);

	bool foundCorners = findChessboardCorners(checkImg, checkSize, corners, CALIB_CB_ADAPTIVE_THRESH);

#ifdef _DEBUG
	// Draw corners on image 
	Mat displayImg = checkImg.clone();
	drawChessboardCorners(displayImg, checkSize, corners, foundCorners);

	// Display image for 5 seconds
	imshow("Checkerboard", displayImg);
	waitKey(5000);
#endif

	ASSERT_TRUE(foundCorners);
}

/*
TEST(Calibrator, CalculatesCamPosition)
{
	const float err_thres = 0.1f;
	Mat checkImg = imread("data/checkerboard1.png");
	Size checkSize = Size(9, 7);

	ASSERT_TRUE(checkImg.data);
	Mat yuvImg;
	cvtColor(checkImg, yuvImg, COLOR_BGR2YUV);

	Calibrator cal;
	Transformation t = Transformation(
		Vector3f(1, 2, 3),
		Quarternion(1.0, 0, 0, 0)
	);

	cal.Add(YUVImage(yuvImg.data, yuvImg.cols, yuvImg.rows), t, 1, checkSize.height, checkSize.width);
	Calibrator::Constants c = cal.GetConstants();



	int i = c.tvec_cw.type();
	Vector3f calPos = Vector3f(
		(float) c.tvec_cw.at<double>(0),
		(float) c.tvec_cw.at<double>(1),
		(float) c.tvec_cw.at<double>(2)
	);
	cal.Reset();

	ASSERT_NEAR(calPos.x, t.loc.x, err_thres);
	ASSERT_NEAR(calPos.y, t.loc.y, err_thres);
	ASSERT_NEAR(calPos.z, t.loc.z, err_thres);
}
*/

TEST(Calibrator, SmallReprojectionError)
{
	Mat checkImg = imread("data/checkerboard1.png");
	Size checkSize = Size(9, 7);

	ASSERT_TRUE(checkImg.data);
	Mat yuvImg;
	cvtColor(checkImg, yuvImg, COLOR_BGR2YUV);

	Calibrator cal;
	double reprojErr;

	cal.Add(YUVImage(yuvImg.data, yuvImg.cols, yuvImg.rows), Transformation(), 1, checkSize.height, checkSize.width);
	reprojErr = cal.GetConstants().reprojectionErr;
	cal.Reset();

	ASSERT_LE(reprojErr, 1.0f);
}

TEST(Calibrator, MultipleAddsWithSameError)
{
	Mat checkImg = imread("data/checkerboard1.png");
	Size checkSize = Size(9, 7);

	ASSERT_TRUE(checkImg.data);
	Mat yuvImg;
	cvtColor(checkImg, yuvImg, COLOR_BGR2YUV);

	Calibrator cal;
	double reprojErr1, reprojErr2;

	cal.Add(YUVImage(yuvImg.data, yuvImg.cols, yuvImg.rows), Transformation(), 1, checkSize.height, checkSize.width);
	reprojErr1 = cal.GetConstants().reprojectionErr;							  
	cal.Add(YUVImage(yuvImg.data, yuvImg.cols, yuvImg.rows), Transformation(), 1, checkSize.height, checkSize.width);
	reprojErr2 = cal.GetConstants().reprojectionErr;;
	cal.Reset();

	ASSERT_NEAR(reprojErr1 - reprojErr2, 0.0f, 0.01f);
}