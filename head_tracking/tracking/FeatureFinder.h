#ifndef MARKER_FINDER_H
#define MARKER_FINDER_H

#include "Feature.h"
#include "utility\Vector3f.h"
#include "utility\YUVImage.h"
#include "utility\Rectangle.h"
#include <vector>

using namespace std;

class FeatureFinder
{
public:

	FeatureFinder()
	{

	}
	uint8_t intensityThreshold = 80;
	//const uint8_t intensityThreshold = 80;
	
	FeatureFinder(uint8_t intensityThreshold);
	vector<Feature> FindFeatures(const YUVImage & image, Rectangle region);

	void FillChecked(const YUVImage & image, bool * checkarray,
		Rectangle region, tuple<int, int> start);
	vector<tuple<int, int>> GetFeaturePixels(const YUVImage & image,
		Rectangle region, tuple<int, int> start);
	Feature FeatureFinder::DefineFeature(const YUVImage & image,
		Rectangle region, tuple<int, int> start);
private:
	class PotentialFeature
	{
		
	};
};

#endif /* MARKER_FINDER_H */