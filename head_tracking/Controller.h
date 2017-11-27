#pragma once
#include "Calibrator.h"
#include "utility/YUVImage.h"
#include "utility/Transformation.h"
#ifdef __GNUC__
	#include "com/RpiSerial.h"
#else
	#include "mock/MockSerial.h"
#endif
#include "com/CommandParser.h"
#include "ControllerInterface.h"
#include "tracking/MarkerTracker.h"
#include <condition_variable>
#include <mutex>

using namespace std;

class CommandParser;

class Controller : public ControllerInterface
{

public:
	Controller();
	Controller(vector<Vec3f> trackModel);
	~Controller();

	Vector3f ProcessCameraFrame(const YUVImage image);
	void ResetCalibration(int cameraNum);
	bool AddCalibration(int cameraNum, Transformation t, float checkerSize, int checkerRows, int checkerCols);
	void UpdatePosition(Vector3f position);

private:

#ifdef __GNUC__
	RpiSerial serial;
#else
	MockSerial serial;
#endif
	const static vector<Vec3f> defaultTrackModel;
	const vector<Vec3f> trackModel;
	Calibrator calibrator;
	CommandParser* cmdParser;
	MarkerTracker tracker;

	YUVImage * calImg = nullptr;
	uint8_t * calImgData = nullptr;
	mutex calImgWait_m;
	condition_variable calImgWait_cv;
	bool calWaitingForImage = false;

	void HandleImageForCalibration(const YUVImage img);
};

