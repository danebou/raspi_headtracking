#pragma once
#include "utility/Transformation.h"

class Calibrator
{
public:
	bool isCalibrated;

	Calibrator();
	void Reset();
	void Add(Transformation transformation, float checkerSize, int checkerRows, int checkerCols);

private:
	int count;

	vector<vector<Vector3>> GenerateCheckerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols);
};

