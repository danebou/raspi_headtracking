#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "MarkerFinder.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

void makeImageFromBool(const bool * bArray, YUVImage ** out, int width, int height)
{
	uint8_t * buffer = (uint8_t *) calloc(width * height, sizeof(YUVImage::YUVCoord));
	*out = new YUVImage(buffer, width, height);
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			YUVImage::YUVCoord coord = YUVImage::YUVCoord(bArray[x + width * y] ? 0xFF : 0x00, 0x00, 0x00);
			((YUVImage::YUVCoord *) buffer)[x + width * y] = coord;
		}
	}
}

TEST(MarkerFinder, FillChecked)
{
	bool expected[] = {
		1, 0, 0, 0, 0,
		1, 1, 1, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 1,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	};
	bool actual[5 * 6] = { false };

    YUVImage * image;
	makeImageFromBool(expected, &image, 5, 6);

	MarkerFinder mF = MarkerFinder();

	mF.FillChecked(*image, actual, Rectangle(0,0,5,6), make_tuple(0,0));
	ASSERT_THAT(expected, ::testing::ContainerEq(actual));

	free(image->data);
	delete image;
}

TEST(MarkerFinder, FindsSingleMarker)
{
	bool bImage[] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	MarkerFinder::Marker expected = { MarkerFinder::Marker(2, 2, 1) };

	YUVImage * image;
	makeImageFromBool(bImage, &image, 5, 6);

	MarkerFinder mF = MarkerFinder();
	vector<MarkerFinder::Marker> markers = mF.FindMarkers(*image, Rectangle(0, 0, 5, 6));
	ASSERT_EQ(1, markers.size());
	ASSERT_EQ(expected.x, markers[0].x);
	ASSERT_EQ(expected.y, markers[0].y);
	ASSERT_NEAR(expected.size, markers[0].size, 2);

	free(image->data);
	delete image;
}

TEST(MarkerFinder, Something)
{
	VideoCapture cap("C:\\Users\\daneb\\Source\\Repos\\raspi_headtracking\\test\\vsproject\\HeadTracking_Test\\x64\\Debug\\ir_tracking_stress.avi"); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return;

	MarkerFinder mF = MarkerFinder();

	Mat yuv, gray;
	namedWindow("gray", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, yuv, COLOR_BGR2YUV);

		int dataLength = yuv.size().width * yuv.size().height;
		int width = yuv.size().width;
		int height = yuv.size().height;

		YUVImage img = YUVImage((uint8_t *)yuv.data, width, height);
		vector<MarkerFinder::Marker> markers = mF.FindMarkers(img, Rectangle(0, 0, width, height));

		cvtColor(frame, gray, COLOR_BGR2GRAY);
		cvtColor(gray, gray, COLOR_GRAY2BGR);
		for (MarkerFinder::Marker i : markers)
		{
			circle(gray, CvPoint((int)i.x, (int)i.y), (int)i.size, Scalar(0, 255, 0));
		}
		imshow("gray", gray);
		if (waitKey(30) >= 0) break;
	}



	// the camera will be deinitialized automatically in VideoCapture destructor
	return ;
}