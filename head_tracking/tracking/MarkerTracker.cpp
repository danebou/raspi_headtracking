#include "MarkerTracker.h"
#include "opencv2\opencv.hpp"

MarkerTracker::MarkerTracker()
{
}

Transformation MarkerTracker::FindMarker(const YUVImage & image, Rectangle region)
{

	vector<Feature> features = ff.FindFeatures(image, Rectangle(0, 0, image.width, image.height));

	//solveP3P()

	Quarternion rot;
	Vector3f loc;

	return Transformation(loc, rot);
}
