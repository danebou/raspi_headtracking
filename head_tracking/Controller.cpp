#include "Controller.h"
#include <mutex>

using namespace std;

const vector<Vec3f> Controller::trackModel = 
{
	Vec3f(-0.01f, 0.00f, 0.00f),
	Vec3f(0.00f, 0.01f, 0.00f),
	Vec3f(0.01f, 0.00f, 0.00f),
	Vec3f(0.00f, 0.01f, 0.00f)
};

Controller::Controller()
	: tracker(calibrator,trackModel)
{
	cmdParser = new CommandParser(*this, serial);
}

Controller::~Controller()
{
	delete cmdParser;
	cmdParser = nullptr;

	delete calImg;
	calImg = nullptr;
	delete [] calImgData;
	calImgData = nullptr;
}

void Controller::ProcessCameraFrame(YUVImage image)
{
	HandleImageForCalibration(image);
}

void Controller::UpdatePosition(Vector3f position)
{
	cmdParser->UpdatePosition(position);
}

void Controller::ResetCalibration(int cameraNum)
{
	(void) cameraNum; // Placeholder parameter, avoid warning
	calibrator.Reset();
}

void Controller::AddCalibration(int cameraNum, Transformation t, float checkerSize, int checkerRows, int checkerCols)
{
	(void) cameraNum; // Placeholder parameter, avoid warning

	// Request lock 
	unique_lock<mutex> mlock(calImgWait_m);

	// Exit if we were alreadying performing a calibration
	// Ignore this request
	if (calWaitingForImage)
		return; // (automatically unlocked)

	// We are now waiting on an image
	calWaitingForImage = true;

	// Wait to retrieve image
	calImgWait_cv.wait(mlock, [this] {return !this->calWaitingForImage; });
	// (we now have a valid calImg!)

	// Perform calibration
	calibrator.Add(*calImg, t, checkerSize, checkerRows, checkerCols);
}

void Controller::HandleImageForCalibration(const YUVImage image)
{
	// Acquire lock for waiting mutex
	lock_guard<mutex> mlock(calImgWait_m);

	// Don't do anything if we did not request and image for calibration
	if (!calWaitingForImage)
		return;

	// Delete old image
	delete calImg;
	calImg = nullptr;
	delete [] calImgData;
	calImgData = nullptr;

	// Copy new image data;
	size_t imgDataLen = image.width * image.height * 3;
	calImgData = new uint8_t[imgDataLen]; // Allocate new memory for data to be copied to
	memcpy(calImgData, image.data, imgDataLen); // Copy data
	calImg = new YUVImage(calImgData, image.width, image.height); // Create new YUV image from copied data

	// No longer waiting for lock
	calWaitingForImage = false;

	// Signal CV 
	calImgWait_cv.notify_one();

	// (lock is destroyed, and mutex is unlocked)
}