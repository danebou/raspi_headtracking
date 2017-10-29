#pragma once
#include "Calibrator.h"
#include "utility/YUVImage.h"
#include "utility/Transformation.h"
#include "com/RpiSerial.h"
#include "com/CommandParser.h"
#include "ControllerInterface.h"

class CommandParser;

class Controller : public ControllerInterface
{

public:
	Controller();
	~Controller();

	void ProcessCameraFrame(YUVImage image);
	void ResetCalibration(int cameraNum);
	void AddCalibration(int cameraNum, Transformation t, float checkerSize);
	void UpdatePosition(Vector3f position);
	void ReceiveCommand();

private:

	RpiSerial serial;
	Calibrator calibrator;
	CommandParser* cmdParser;

};

