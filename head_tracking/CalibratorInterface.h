#pragma once
#include <vector>
#include "utility/Vector3f.h"
#include "utility/YUVImage.h"
#include "utility/Transformation.h"
#include "utility/Checkerboard.h"
#include <opencv2/opencv.hpp>
#include <mutex>

using namespace std;
using namespace cv;

class CalibratorInterface
{
public:

	struct Constants
	{
		bool isCalibrated;
		Mat camMat;
		Mat distCoeffs;
		Mat rvec_cw, tvec_cw;
		double reprojectionErr;
	};

	virtual void Reset()=0;
	virtual bool Add(YUVImage img, Transformation transformation, float checkerSize, int checkerRows, int checkerCols)=0;
	virtual Constants GetConstants()=0;
};
