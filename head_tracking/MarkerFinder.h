#ifndef MARKER_FINDER_H
#define MARKER_FINDER_H

#include "utility\Vector3f.h"
#include <vector>
#include "utility\YUVImage.h"
#include "utility\Rectangle.h"

using namespace std;

class MarkerFinder
{
public:
	class Marker
	{
	public:
		float x, y;
		float size;
	};

	MarkerFinder()
	{

	}
	uint8_t intensityThreshold = 80;
	//const uint8_t intensityThreshold = 80;
	
	MarkerFinder(uint8_t intensityThreshold);
	vector<Marker> FindMarkers(const YUVImage & image, Rectangle region);

	void FillChecked(const YUVImage & image, bool * checkarray,
		Rectangle region, tuple<int, int> start);
	vector<tuple<int, int>> GetMarkerPixels(const YUVImage & image,
		Rectangle region, tuple<int, int> start);
	Marker MarkerFinder::DefineMarker(const YUVImage & image,
		Rectangle region, tuple<int, int> start);
private:
	class PotentialMarker
	{
		
	};
};

#endif /* MARKER_FINDER_H */