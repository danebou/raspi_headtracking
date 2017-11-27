#pragma once
#include "CalibratorInterface.h"

class MockCalibrator :
	public CalibratorInterface
{
public:
	MockCalibrator();
	~MockCalibrator();

	Constants constants;

	void Reset();
	bool Add(YUVImage img, Transformation transformation, float checkerSize, int checkerRows, int checkerCols);
	Constants GetConstants();

};

