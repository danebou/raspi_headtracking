/*
	The Marker tracer keeps track of markers on a rigid body
*/
#pragma once
#include "FeatureFinder.h"
#include "utility/Transformation.h"
#include <vector>

using namespace std;

class MarkerTracker
{
public:
	MarkerTracker();

	Transformation FindMarker(const YUVImage & image, Rectangle region);

private:
	FeatureFinder ff;
};

