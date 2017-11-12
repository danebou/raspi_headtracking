#include "Controller.h"
#include "com/RpiSerial.h"
#include <thread>

Controller::Controller()
{
	cmdParser = new CommandParser(*this, serial);
}

Controller::~Controller()
{
	delete cmdParser;
}

void Controller::ProcessCameraFrame(YUVImage image)
{
	(void) image;
}

void Controller::UpdatePosition(Vector3f position)
{
	cmdParser->UpdatePosition(position);
}

void Controller::ResetCalibration(int cameraNum)
{
	(void) cameraNum;
	calibrator.Reset();
}

void Controller::AddCalibration(int cameraNum, Transformation t, float checkerSize, int checkerRows, int checkerCols)
{
	(void) cameraNum;
	calibrator.Add(t, checkerSize, checkerRows, checkerCols);
}