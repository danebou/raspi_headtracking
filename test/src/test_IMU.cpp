#include <gtest\gtest.h>
#include "imu_sensor.h"

// Tests 
TEST(Vector3f, HandlesZeroInput)
{
	Vector3f v;
	EXPECT_EQ(0, v.x);
	EXPECT_EQ(0, v.y);
	EXPECT_EQ(0, v.z);
}