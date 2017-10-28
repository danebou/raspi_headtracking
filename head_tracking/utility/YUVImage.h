#ifndef YUVIMAGE_H
#define YUVIMAGE_H

#include <stdint.h>
#include <vector>
#include <opencv2\core.hpp>

using namespace std;
using namespace cv;

class YUVImage
{
public:
    class YUVCoord
    {
	public:
        uint8_t y, u, v;

		YUVCoord()
		{
			y = 0;
			u = 0;
			v = 0;
		}
		YUVCoord(uint8_t y, uint8_t u, uint8_t v)
			: u(u), y(y), v(v)
		{
		}
	};

	int width, height;
	YUVCoord * data;

	YUVImage(uint8_t * imageBuffer, int width, int height);
	inline YUVCoord GetCoordinate(int x, int y) const
	{
		return data[y * width + x];
	}
};

#endif /* YUVIMAGE_H */