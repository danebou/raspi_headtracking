#pragma once
#include "utility/Transformation.h"
#include "utility/Vector3f.h"
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Checkerboard
{
public:
	const float size;
	const int rows;
	const int cols;

	Checkerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols);
	Checkerboard(float checkerSize, int checkerRows, int checkerCols);

	void ApplyTransformation(Transformation t);
	vector<Point3f> GetInnerCorners();

private:
	vector<vector<Vector3f>> corners;
};

