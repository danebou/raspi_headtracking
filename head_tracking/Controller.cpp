#include "Controller.h"

enum class CommandType {
	PositionData = 0x00,
	PrenticeSync = 0x01,
	SetEyeDistance = 0x00,
	SetLatencyCompensation = 0x02,
	MasterStartCalibrate = 0x01,
	MasterAddCalbirate = 0x02,
	PrenticeStartCalibrate = 0x02,
	PrenticeAddCalbirate = 0x03,
	PrenticeTrackingData = 0x00,
	PrenticeTrackingSync = 0x00,
	PrenticeGetCameraMatrix = 0x01,
	PrenticeCameraMatrix = 0x02,
};

Controller::Controller()
{
}

void DoCommand(CommandType type)
{
	switch (type)
	{
	case CommandType::PositionData:
		break;
	}
}