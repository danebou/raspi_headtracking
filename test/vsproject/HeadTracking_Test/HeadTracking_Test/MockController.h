#pragma once
#include "ControllerInterface.h"

class MockController : public ControllerInterface
{
public:
	void ProcessCameraFrame(YUVImage image);
	void ResetCalibration(int cameraNum);
	void AddCalibration(int cameraNum, Transformation t, float checkerSize);
};

