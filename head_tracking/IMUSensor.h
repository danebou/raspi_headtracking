#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#endif // !IMU_SENSOR_H

#include <chrono>
#include "utility\vector3f.h"

using namespace std;

class IMUSensor
{

public:
	struct IMUData
	{
		Vector3f pos;
		Vector3f vel;
		Vector3f accl;

	};

	chrono::milliseconds GetLatency();
	void Request_UpdateBy(chrono::time_point<chrono::);
	ImuData GetPos();
};

#endif /* IMU_SENSOR_H */