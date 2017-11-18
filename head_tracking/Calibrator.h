#pragma once
#include <vector>
#include "utility/Vector3f.h"
#include "utility/Transformation.h"

using namespace std;

class Calibrator
{
public:
	bool isCalibrated;

	Calibrator();
	void Reset();
	void Add(Transformation transformation, float checkerSize, int checkerRows, int checkerCols);
	static vector<vector<Vector3f>> GenerateCheckerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols);

private:
	int count;

};

