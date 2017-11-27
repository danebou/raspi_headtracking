#pragma once
#include "CalibratorInterface.h"

using namespace std;
using namespace cv;

class Calibrator : public CalibratorInterface
{
public:

	Calibrator();
	void Reset();
	bool Add(YUVImage img, Transformation transformation, float checkerSize, int checkerRows, int checkerCols);
	Constants GetConstants();

private:
	Constants constants;
	int count;
	vector<Point2f> calImgPoints;
	vector<Point3f> calObjPoints;
	mutex constant_m;

	double CalibrateIntrinsicParameters(vector<Point3f> objPoints, vector<Point2f> imgPoints, Size imgSize);
	double CalibrateExtrinsicParameters(vector<Point3f> objPoints, vector<Point2f> imgPoints);
	vector<Point2f> GetCheckerCornersFromImage(YUVImage image, Checkerboard cb);
	double GetReprojectionError();
	void InvertRT(InputArray rvec, InputArray tvec, OutputArray rvec_i, OutputArray tvec_i);
};

