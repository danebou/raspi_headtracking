#include "MockCalibrator.h"



MockCalibrator::MockCalibrator()
{
}


MockCalibrator::~MockCalibrator()
{
}

void MockCalibrator::Reset()
{
}

bool MockCalibrator::Add(YUVImage img, Transformation transformation, float checkerSize, int checkerRows, int checkerCols)
{
	return false;
}

MockCalibrator::Constants MockCalibrator::GetConstants()
{
	return constants;
}
