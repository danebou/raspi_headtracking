#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tracking/FeatureFinder.h"
#include "tracking/Feature.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
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

TEST(FeatureFinder, FillChecked)
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

	FeatureFinder mF = FeatureFinder();

	mF.FillChecked(*image, actual, Rectangle(0,0,5,6), make_tuple(0,0));
	ASSERT_THAT(expected, ::testing::ContainerEq(actual));

	free(image->data);
	delete image;
}

TEST(FeatureFinder, FindsSingleFeature)
{
	bool bImage[] = {
		0, 0, 0, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	Feature expected = { Feature(2, 2, 1) };

	YUVImage * image;
	makeImageFromBool(bImage, &image, 5, 6);

	FeatureFinder mF = FeatureFinder();
	vector<Feature> features = mF.FindFeatures(*image, Rectangle(0, 0, 5, 6));
	ASSERT_EQ(1, features.size());
	ASSERT_EQ(expected.x, features[0].x);
	ASSERT_EQ(expected.y, features[0].y);
	ASSERT_NEAR(expected.size, features[0].size, 2);

	free(image->data);
	delete image;
}

void FeatureFinderVideoTest(string fileName, vector<int> nonTrackFrames, int featureCount = -1)
{
	// Open video
	VideoCapture cap(fileName); // open the default camera
	if (!cap.isOpened()) { // check if we succeeded
		FAIL();
		return;
	}
	int frameCount = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);

	FeatureFinder mF = FeatureFinder();
	Mat frame, yuv, gray;
#ifdef _DEBUG // Only debug mode shows image output window
	// Create window
	namedWindow("IR Tracking Stress", CV_WINDOW_AUTOSIZE);
#endif

	// Loop over video frames
	for (;;)
	{
		int frameIndex = (int)cap.get(CV_CAP_PROP_POS_FRAMES);
		cap >> frame; // Get a new frame from video
		if (frame.empty()) // Check and end if no more frames
			break;

		// Convert to YUV color-space
		cvtColor(frame, yuv, COLOR_BGR2YUV);

		int width = yuv.size().width;
		int height = yuv.size().height;

		// Find features
		YUVImage img = YUVImage((uint8_t *)yuv.data, width, height);
		vector<Feature> features = mF.FindFeatures(img, Rectangle(0, 0, width, height));

		// Did we find any features?
		bool found = features.size() > 0;
		bool foundAll = found & (featureCount == -1 || features.size() >= featureCount);
		bool frameHasFeature = find(nonTrackFrames.begin(), nonTrackFrames.end(), frameIndex)
			== nonTrackFrames.end();

#ifdef _DEBUG
		// Grayscale image
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		cvtColor(gray, gray, COLOR_GRAY2BGR);

		// Draw features
		for (Feature i : features)
			circle(gray, CvPoint((int)i.x, (int)i.y), (int)i.size, Scalar(0, 255, 0));

		// Draw sad text if there are no features
		if (!found && frameHasFeature)
			putText(gray, "Tracker not found!", Point(width / 2, height / 2),
				FONT_HERSHEY_SIMPLEX, 1.0f, Scalar(0, 0, 255), 3);
		if (!foundAll && frameHasFeature)
			putText(gray, "Could not find all features", Point(width / 2, height / 2),
				FONT_HERSHEY_SIMPLEX, 1.0f, Scalar(0, 0, 255), 3);

		// Draw frame index
		stringstream frameDescStr;
		frameDescStr << "Frame index " << frameIndex << "/" << frameCount;

		putText(gray, frameDescStr.str(), Point(width / 2, height / 2),
			FONT_HERSHEY_SIMPLEX, 1.0f, Scalar(255, 255, 255), 3);

		imshow("IR Tracking", gray);

		// Wait to display next frame, (wait a full second for failed)
		if (waitKey(foundAll || !frameHasFeature ? 1 : 1000) >= 0)
		{
			FAIL();
			break;
		}
#endif

		ASSERT_EQ(frameHasFeature, foundAll) << "Frame :" << frameIndex;
	}
#ifdef _DEBUG
	destroyWindow("IR Tracking Stress");
#endif
}

TEST(FeatureFinder, FindsFeatureEasy)
{
	const vector<int> nonTrackFrames = // Frames that do not have a track point visible
	{
		0, 1, 2, 1757, 1758, 1759, 1760, 1761
	};
	FeatureFinderVideoTest("data/ir_tracking_easy.avi", nonTrackFrames);
}

TEST(FeatureFinder, FindsFeatureStress)
{
	const vector<int> nonTrackFrames = // Frames that do not have a track point visible
	{
		0, 1, 2, 1134, 1135, 1136, 1137, 1644, 1645,
		1646, 1647, 1648, 1649, 1650, 1651, 1652,
		1653, 1654
	};
	FeatureFinderVideoTest("data/ir_tracking_stress.avi", nonTrackFrames);
}

TEST(FeatureFinder, FindsFeature4Point)
{
	
	const vector<int> nonTrackFrames = // Frames that do not have a track point visible
	{
	};
	FeatureFinderVideoTest("data/ir_tracking_4point.avi", nonTrackFrames, 4);
}