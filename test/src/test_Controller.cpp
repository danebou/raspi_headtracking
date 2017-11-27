#include <gtest/gtest.h>
#include "ControllerInterface.h"
#include "Controller.h"
#include <thread>
#include <chrono>
#include <functional>
#include <future>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

TEST(Controller, CalibrationSuccessful)
{
	Controller c;

	Transformation t(Vector3f(), Quarternion(1,0,0,0));
	thread sendCalibrationImage_t([&c](Controller & c)
	{
		Mat img = imread("data/checkerboard1.png");
		ASSERT_TRUE(img.data) << "Could not open test image";

		// Wait some time
		this_thread::sleep_for(chrono::milliseconds(4000));

		// Process image
		YUVImage yuvImg = YUVImage(img.data, img.cols, img.rows);
		c.ProcessCameraFrame(yuvImg);
	}, ref(c));

	bool success = c.AddCalibration(0, t, 1, 7, 9);

	// Join Thread
	if (sendCalibrationImage_t.joinable())
		sendCalibrationImage_t.join();

	ASSERT_TRUE(success) << "Image could not be calibrated";
}

TEST(Controller, CalibrationMultiCalibStable)
{
	Controller c;

	Transformation t(Vector3f(), Quarternion(1, 0, 0, 0));
	auto sendCalibrationImage_l = [&c](Controller & c)
	{
		Mat img;
		{
			img = imread("data/checkerboard1.png");
			ASSERT_TRUE(img.data) << "Could not open test image";
		}

		// Wait some time
		this_thread::sleep_for(chrono::milliseconds(4000));

		// Process image
		YUVImage yuvImg = YUVImage(img.data, img.cols, img.rows);
		c.ProcessCameraFrame(yuvImg);
	};
	
	// Is there a fancy way to spinoff threads? (fill, resize, etc.)
	vector<thread> sendCalibrationImage_t;
	for (int i = 0; i < 100; i++)
	{
		sendCalibrationImage_t.push_back(thread(sendCalibrationImage_l, ref(c)));
	}

	vector<future<bool>> addCalibration_p;
	for (int i = 0; i < 100; i++)
	{
		addCalibration_p.push_back(async(&Controller::AddCalibration, &c, 0, t, 1, 7, 9));
	}

	// Join threads
	for_each(sendCalibrationImage_t.begin(), sendCalibrationImage_t.end(),
		[](thread & t) { if (t.joinable()) t.join(); });

	// Wait for remaining values
	for_each(addCalibration_p.begin(), addCalibration_p.end(),
		[](future<bool> & f) { f.wait(); });

	// Make sure at least one calibration was successful
	bool success = any_of(addCalibration_p.begin(), addCalibration_p.end(),
		[](future<bool> & f) { return f.get(); });
		
	ASSERT_TRUE(success) << "Image could not be calibrated";
}

TEST(Controller, DetectsPosition)
{
	const vector<Vec3f> model =
	{
		Vec3f(-0.5f, -0.5f, 0.00f),
		Vec3f(0.5f, -0.5f, 0.00f),
		Vec3f(0.5f, 0.5f, 0.00f),
		Vec3f(-0.5f, 0.5f, 0.00f)
	};
	const Vector3f actualPos = Vector3f(0.5f, 0.5f, 0.0f);

	Controller c(model);

	Transformation t(Vector3f(), Quarternion(1, 0, 0, 0));
	thread sendCalibrationImage_t([&c](Controller & c)
	{
		Mat img = imread("data/checkerboard1.png");
		ASSERT_TRUE(img.data) << "Could not open test image";

		// Wait some time
		this_thread::sleep_for(chrono::milliseconds(4000));

		// Process image
		YUVImage yuvImg = YUVImage(img.data, img.cols, img.rows);
		c.ProcessCameraFrame(yuvImg);
	}, ref(c));

	bool imgCalibrated = c.AddCalibration(0, t, 1, 7, 9);

	// Join Thread
	if (sendCalibrationImage_t.joinable())
		sendCalibrationImage_t.join();

	ASSERT_TRUE(imgCalibrated) << "Image could not be calibrated";

	// Load points image
	Mat img = imread("data/checkerboard1_4points.png");
	ASSERT_TRUE(img.data) << "Could not open test image";

	// Process image
	YUVImage yuvImg = YUVImage(img.data, img.cols, img.rows);
	Vector3f calcPos = c.ProcessCameraFrame(yuvImg);

	ASSERT_NEAR((calcPos - actualPos).magnitude(), 0, 1);
}