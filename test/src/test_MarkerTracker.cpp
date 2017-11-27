#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tracking/MarkerTracker.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "mock/MockCalibrator.h"

using namespace cv;

TEST(MarkerTracker, FindsMarker)
{
	const vector<int> nonTrackFrames = {};
	const string filename = "data/ir_tracking_4point.avi";

	const vector<Vec3f> model =
	{
		Vec3f(-0.5f, -0.5f, 0.00f),
		Vec3f(0.5f, -0.5f, 0.00f),
		Vec3f(0.5f, 0.5f, 0.00f),
		Vec3f(-0.5f, 0.5f, 0.00f)
	};

	MockCalibrator c = MockCalibrator();
	c.constants.camMat = (Mat_<double>(3, 3) << 1, 0, 1, 0, 1, 1, 0, 0, 1);
	c.constants.distCoeffs = (Mat_<double>(1, 5) << 0, 0, 0, 0, 0);
	c.constants.rvec_cw = (Mat_<double>(3, 1) << 1, 0, 0);
	c.constants.tvec_cw = (Mat_<double>(3, 1) << 0, 0, 0);

	MarkerTracker mt = MarkerTracker(c, model);

	// Open video
	VideoCapture cap(filename); // open the default camera
	ASSERT_TRUE(cap.isOpened()); // check if we succeeded
	int frameCount = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
	Mat frame, yuv, gray;
#ifdef _DEBUG // Only debug mode shows image output window
	// Create window
	namedWindow("IR Tracking Marker", CV_WINDOW_AUTOSIZE);
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

		Vector3f pos = mt.FindLocation(img);

		// Did we find any features?
		bool found = (pos.x != NAN) && pos.y != NAN && pos.z != NAN;
		bool frameHasFeature = find(nonTrackFrames.begin(), nonTrackFrames.end(), frameIndex)
			== nonTrackFrames.end();

		ASSERT_EQ(frameHasFeature, found) << "value of vector3: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << endl;

#ifdef _DEBUG
		// Grayscale image
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		cvtColor(gray, gray, COLOR_GRAY2BGR);

		// Draw features
		if (found)
		{
			// TODO reproject
			//circle(gray, CvPoint((int)i.x, (int)i.y), (int)i.size, Scalar(0, 255, 0));
		}

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