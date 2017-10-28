#include "Controller.h"
#include "com/RpiSerial.h"
#include <thread>

Controller::Controller()
{
	serial = new RpiSerial();
	calibrator = Calibrator();
	cmdParser = new CommandParser(*this, *serial);
}

Controller::~Controller()
{
	delete cmdParser;
	delete serial;
}

void Controller::ProcessCameraFrame(YUVImage image)
{
}

void Controller::UpdatePosition(Vector3f position)
{
	cmdParser->UpdatePosition(position);
}

void Controller::ResetCalibration(int cameraNum)
{
	calibrator.Reset();
}

void Controller::AddCalibration(int cameraNum, Transformation t, float checkerSize)
{
	calibrator.Add(t, checkerSize);
}