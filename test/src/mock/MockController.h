#pragma once
#include "ControllerInterface.h"

class MockController : public ControllerInterface
{
public:
	Vector3f ProcessCameraFrame(const YUVImage image);
	void ResetCalibration(int cameraNum);
	bool AddCalibration(int cameraNum, Transformation t, float checkerSize);
};

