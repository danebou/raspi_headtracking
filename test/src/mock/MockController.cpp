#include "MockController.h"

Vector3f MockController::ProcessCameraFrame(const YUVImage image)
{
	return Vector3f(NAN, NAN, NAN);
}
void MockController::ResetCalibration(int cameraNum)
{

}

bool MockController::AddCalibration(int cameraNum, Transformation t, float checkerSize)
{
	return false;
}