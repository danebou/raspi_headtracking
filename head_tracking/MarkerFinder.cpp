#include "MarkerFinder.h"
#include <stdlib.h>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <algorithm>

/*
	1. Find per
	2. Find center
	2. Find furthest point
	4. Find Furthest point on opposite side
	5. Somehow find width of blob?
	6. subtact blob_width/2 towards center and return both trackers
*/

void MarkerFinder::FillChecked(const YUVImage & image, bool * checkarray,
	Rectangle region, tuple<int, int> start)
{
	// This function just fills in the rest, it will be optimized out later
	queue<tuple<int, int>> locations;
	locations.push(start);
	while (!locations.empty())
	{
		tuple<int, int> next = locations.front();
		int x = get<0>(next);
		int y = get<1>(next);
		locations.pop();

		if (image.GetCoordinate(x, y).y >= intensityThreshold)
		{
			checkarray[x + y * region.width] = true;
		}

		if (x - 1 >= region.x)
		{
			if (!checkarray[(x - 1)+(y)*region.width] 
				&& image.GetCoordinate(x - 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x - 1, y));
		}
		if (x + 1 < region.x + region.width)
		{
			if (!checkarray[(x + 1)+(y)*region.width]
				&& image.GetCoordinate(x + 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x + 1, y));
		}
		if (y - 1 >= region.y)
		{
			if (!checkarray[(x)+(y - 1)*region.width]
				&& image.GetCoordinate(x, y - 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y - 1));
		}
		if (y + 1 < region.y + region.height)
		{
			if (!checkarray[(x) + (y + 1)*region.width]
				&& image.GetCoordinate(x, y + 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y + 1));
		}
	}
}

vector<tuple<int, int>> MarkerFinder::GetMarkerPixels(const YUVImage & image,
	Rectangle region, tuple<int, int> start)
{
	// This function just fills in the rest, it will be optimized out later
	queue<tuple<int, int>> locations;
	vector<tuple<int, int>> explored;

	locations.push(start);
	while (!locations.empty())
	{
		tuple<int, int> next = locations.front();
		int x = get<0>(next);
		int y = get<1>(next);
		locations.pop();

		if (image.GetCoordinate(x, y).y >= intensityThreshold)
		{
			explored.push_back(make_tuple(x, y));
		}

		if (x - 1 >= region.x)
		{
			if (find(explored.begin(), explored.end(),
				make_tuple(x - 1, y)) == explored.end()
				&& image.GetCoordinate(x - 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x - 1, y));
		}
		if (x + 1 < region.x + region.width)
		{
			if (find(explored.begin(), explored.end(),
				make_tuple(x + 1, y)) == explored.end()
				&& image.GetCoordinate(x + 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x + 1, y));
		}
		if (y - 1 >= region.y)
		{
			if (find(explored.begin(), explored.end(),
				make_tuple(x, y - 1)) == explored.end()
				&& image.GetCoordinate(x, y - 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y - 1));
		}
		if (y + 1 < region.y + region.height)
		{
			if (find(explored.begin(), explored.end(),
				make_tuple(x, y - 1)) == explored.end()
				&& image.GetCoordinate(x, y + 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y + 1));
		}
	}

	return explored;
}

MarkerFinder::Marker MarkerFinder::DefineMarker(const YUVImage & image,
	Rectangle region, tuple<int, int> start)
{
	vector<tuple<int, int>> pixels = GetMarkerPixels(image, region, start);
	return Marker();
}

vector<MarkerFinder::Marker> MarkerFinder::FindMarkers(
	const YUVImage & image, Rectangle region)
{
	vector<Marker> found;
	bool * checked = (bool *) calloc(region.width * region.height, sizeof(bool));

	// Search region for markers
	for (int y = region.y; y < region.height + region.y; y++)
	{
		for (int x = region.x; x < region.width + region.x; x++)
		{
			if (image.GetCoordinate(x, y).y >= intensityThreshold)
			{
				if (!checked[x + y * region.width])
				{
					Marker m = DefineMarker(image, region, make_tuple(x, y));
					found.push_back(m);
					FillChecked(image, checked, region, make_tuple(x, y));
				}
			}
		}
	}

	free(checked);
	return found;
}