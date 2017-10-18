#include <gtest/gtest.h>
#include "utility\Quarternion.h"

// Tests initialization of a quaternion using the default constructor
TEST(Quarternion, HandlesZeroInput)
{
	Quarternion q;
	EXPECT_EQ(0, q.r);
	EXPECT_EQ(0, q.i);
	EXPECT_EQ(0, q.j);
	EXPECT_EQ(0, q.k);
}

// Tests initialization of a quaternion using a 
TEST(Quarternion, HandlesInitInput)
{
	Quarternion q(1, 2, 3, 4);
	EXPECT_EQ(1, q.r);
	EXPECT_EQ(2, q.i);
	EXPECT_EQ(3, q.j);
	EXPECT_EQ(4, q.k);
}

// Tests equality comparison of quaternion
TEST(Quarternion, Equality)
{
	Quarternion a(1, 2, 3, 4);
	Quarternion b(5, 2, 3, 4);
	Quarternion c = a;

	EXPECT_EQ(a, a);
	EXPECT_EQ(a, c);
	EXPECT_NE(a, b);
}

// Tests quaternion addition
TEST(Quarternion, QuarternionAddition)
{
	Quarternion a(1, 2, 3, 4);
	Quarternion b(5, 6, 7, 8);
	Quarternion result(6, 8, 10, 12);
	EXPECT_EQ(result, a + b);
}

// Tests quaternion real addition
TEST(Quarternion, RealAddition)
{
	Quarternion a(1, 2, 3, 4);
	float b = 5;
	Quarternion result(6, 2, 3, 4);
	EXPECT_EQ(result, a + b);
}

// Tests quaternion multiplication
TEST(Quarternion, QuarternionMultiplication)
{
	Quarternion a(-5.0f, 3.0f, 4.0f, 3.0f);
	Quarternion b(4.0f, 3.9f, -1.0f, -3.0f);
	Quarternion result(-18.7f,-16.5f, 41.7f, 8.4f);
	EXPECT_EQ(result, a * b);
}

// Tests quaternion real multiplication
TEST(Quarternion, RealMultiplication)
{
	Quarternion a(1, 2, 3, 4);
	float b = 5;
	Quarternion result(5, 10, 15, 20);
	EXPECT_EQ(result, a * b);
}

// Tests quaternion real divison
TEST(Quarternion, RealDivision)
{
	Quarternion a(1, 2, 3, 4);
	float b = 5;
	Quarternion result(0.2f, 0.4f, 0.6f, 0.8f);
	EXPECT_EQ(result, a / b);
}