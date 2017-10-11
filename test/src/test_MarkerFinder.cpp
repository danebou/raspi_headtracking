#include <gmock/gmock.h>
#include "MarkerFinder.h"
#include <string>
#include <vector>
#include <iostream>

void makeImageFromBool(const bool * bArray, YUVImage ** out, int width, int height)
{
	uint8_t * buffer = (uint8_t *) calloc(width * height, sizeof(YUVImage::YUVCoord));
	*out = new YUVImage(buffer, width, height);
	
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			YUVImage::YUVCoord coord = YUVImage::YUVCoord(bArray[x + width * y] ? 0xFF : 0x00, 0x00, 0x00);
			((YUVImage::YUVCoord *) buffer)[x + width * y] = coord;
		}
	}
}

TEST(MarkerFinder, FillWorks)
{
	bool expected[] = {
		1, 0, 0, 0, 0,
		1, 1, 1, 0, 0,
		0, 1, 1, 1, 0,
		0, 1, 1, 1, 1,
		0, 0, 1, 1, 0,
		0, 0, 0, 0, 0
	};
	bool actual[5 * 6] = { false };
    YUVImage * image;

	makeImageFromBool((bool *) &expected[0], &image, 5, 6);

	MarkerFinder mF = MarkerFinder();

	mF.FillChecked(*image, actual, Rectangle(0,0,5,6), make_tuple(0,0));
	ASSERT_THAT(expected, ::testing::ContainerEq(actual));

	free(image->data);
	delete image;
}