#include "YUVImage.h"

YUVImage::YUVImage(uint8_t * imageBuffer, int width, int height)
	: width(width), height(height), data((YUVCoord *) imageBuffer)
{
}
