#include "YUVImage.h"

YUVImage::YUVImage(uint8_t * imageBuffer, int width, int height)
	: data((YUVCoord *) imageBuffer), width(width), height(height)
{
}
