#include <gtest\gtest.h>
#include "utility\Vector3f.h"

// Tests initialization of a vector using the default constructor
TEST(Vector3f, HandlesZeroInput)
{
	Vector3f v;
    EXPECT_EQ(0, v.x);
	EXPECT_EQ(0, v.y);
	EXPECT_EQ(0, v.z);
}

// Tests initialization of a vector using a 
TEST(Vector3f, HandlesInitInput)
{
	Vector3f v(1, 2, 3);
	EXPECT_EQ(1, v.x);
	EXPECT_EQ(2, v.y);
	EXPECT_EQ(3, v.z);
}

// Tests equality comparison of vector
TEST(Vector3f, Equality)
{
	Vector3f a(1, 2, 3);
	Vector3f b(4, 5, 6);
	Vector3f c = a;

	EXPECT_EQ(a, a);
	EXPECT_EQ(a, c);
	EXPECT_NE(a, b);
}

// Tests vector addition
TEST(Vector3f, Addition)
{
	Vector3f a(1, 2, 3);
	Vector3f b(4, 5, 6);
	Vector3f result(5, 7, 9);
	EXPECT_EQ(result, a + b);
}

// Tests vector addition
/*TEST(Vector3f, ConstantMultiply)
{
	Vector3f a(1, 2, 3);
	const float c = 2;
	Vector3f result(2, 4, 6);
	EXPECT_EQ(result, a * c);
}*/