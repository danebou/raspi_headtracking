/*
	The Marker tracer keeps track of markers on a rigid body
*/
#pragma once
#include "FeatureFinder.h"
#include "utility/Transformation.h"
#include "Calibrator.h"
#include <vector>

using namespace std;

class MarkerTracker
{
public:
	MarkerTracker(Calibrator & calibrator, vector<Vec3f> model);

	Vector3f FindLocation(const YUVImage & image);

private:
	const vector<Vec3f> model;
	FeatureFinder ff;
	Calibrator & calibrator;

	vector<Feature> TrackFeatures(const YUVImage & image);
};

