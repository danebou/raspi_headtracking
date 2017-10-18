#include "imu_sensor.h"

chrono::milliseconds ImuSensor::GetLatency()
{
	return chrono::microseconds(5);
}
