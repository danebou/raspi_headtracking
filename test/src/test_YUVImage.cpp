#include <gtest/gtest.h>
#include "utility/YUVImage.h"

// Tests initialization of a vector using a 
TEST(YUVImage, GetCoords)
{
	YUVImage::YUVCoord imageBuffer[] =
	{
		YUVImage::YUVCoord(1,2,3),
		YUVImage::YUVCoord(4,5,6),
	};
	YUVImage image((uint8_t *)imageBuffer, 1, 2);
	EXPECT_EQ(1, image.GetCoordinate(0, 0).y);
	EXPECT_EQ(2, image.GetCoordinate(0, 0).u);
	EXPECT_EQ(3, image.GetCoordinate(0, 0).v);
	EXPECT_EQ(4, image.GetCoordinate(0, 1).y);
	EXPECT_EQ(5, image.GetCoordinate(0, 1).u);
	EXPECT_EQ(6, image.GetCoordinate(0, 1).v);
}