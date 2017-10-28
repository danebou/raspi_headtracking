#pragma once
#include "utility\Transformation.h"

class Calibrator
{
public:
	bool isCalibrated = false;

	Calibrator();
	void Reset();
	void Add(Transformation transformation, float checkerSize);

private:
	int count = 0;
};

