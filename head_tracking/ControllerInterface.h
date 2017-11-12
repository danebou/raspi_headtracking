#pragma once
#include "Calibrator.h"
#include "utility/YUVImage.h"
#include "utility/Transformation.h"
#include "com/SerialInterface.h"
#include "com/CommandParser.h"

class ControllerInterface
{
public:
	virtual void ProcessCameraFrame(YUVImage image)=0;
	virtual void ResetCalibration(int cameraNum)=0;
	virtual void AddCalibration(int cameraNum, Transformation t, float checkerSize, int checkerRows, int checkerCols)=0;
	virtual void UpdatePosition(Vector3f position)=0;
};

