#include <gmock\gmock.h>
#include <gtest\gtest.h>
#include "tracking/FeatureFinder.h"
#include "tracking/Feature.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

/*
void FeatureFinderVideoTest(string fileName, vector<int> nonTrackFrames)
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
		bool frameHasFeature = find(nonTrackFrames.begin(), nonTrackFrames.end(), frameIndex)
			== nonTrackFrames.end();

		ASSERT_EQ(frameHasFeature, found);

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

		// Draw frame index
		stringstream frameDescStr;
		frameDescStr << "Frame index " << frameIndex << "/" << frameCount;

		putText(gray, frameDescStr.str(), Point(width / 2, height / 2),
			FONT_HERSHEY_SIMPLEX, 1.0f, Scalar(255, 255, 255), 3);

		imshow("IR Tracking", gray);

		// Wait to display next frame, (wait a full second for failed)
		if (waitKey(found || !frameHasFeature ? 1 : 1000) >= 0)
		{
			FAIL();
			break;
		}
#endif
	}
#ifdef _DEBUG
	destroyWindow("IR Tracking Stress");
#endif
}

TEST(MarkerTracker, FindsPoints)
{
	const vector<int> nonTrackFrames = // Frames that do not have a track point visible
	{
		0, 1, 2, 1757, 1758, 1759, 1760, 1761
	};
	FeatureFinderVideoTest("data/ir_tracking_easy.avi", nonTrackFrames);
}*/