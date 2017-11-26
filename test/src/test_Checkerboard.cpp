#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <utility/Checkerboard.h>
#include <opencv2/opencv.hpp>

TEST(Checkerboard, Generates3dCheckerboard)
{
	vector<Point3f> expectedChecker =
	{
		Point3f(-0.5f,  0.5f,  3.0f), Point3f(-2.0f,  0.5f,  3.0f),
		Point3f(-0.5f, -1.0f,  3.0f), Point3f(-2.0f, -1.0f,  3.0f),
		Point3f(-0.5f, -2.5f,  3.0f), Point3f(-2.0f, -2.5f,  3.0f),
	};
	Size checkerSize = Size(4, 5);
	Transformation t = Transformation
	(
		Vector3f(1.0f, 2.0f, 3.0f),
		Quarternion(0, 0, 0, 1) // 180 degrees around z axis (-x, -z)
	);

	Checkerboard c = Checkerboard(t, 1.5f, (int)(checkerSize.height - 1), (int)(checkerSize.width - 1));

	ASSERT_THAT(c.GetInnerCorners(), expectedChecker);
}