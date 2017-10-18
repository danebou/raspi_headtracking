/*
	The Marker tracer keeps track of markers on a rigid body
*/
#pragma once
#include "FeatureFinder.h"
#include <vector>

using namespace std;

class MarkerTracker
{
public:
	MarkerTracker();

private:
	FeatureFinder featureFind;
};

