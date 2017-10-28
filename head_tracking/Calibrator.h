#pragma once
#include "utility/Transformation.h"

class Calibrator
{
public:
	bool isCalibrated;

	Calibrator();
	void Reset();
	void Add(Transformation transformation, float checkerSize);

private:
	int count;
};

