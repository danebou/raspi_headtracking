#define _USE_MATH_DEFINES
#include <cmath>
#include "FeatureFinder.h"
#include <stdlib.h>
#include <tuple>
#include <queue>
#include <unordered_map>
#include <algorithm>

void FeatureFinder::FillChecked(const YUVImage & image, bool * checkarray,
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

		if (checkarray[x + y * region.width])
			continue;

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

vector<tuple<int, int>> FeatureFinder::GetFeaturePixels(const YUVImage & image,
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

		if (find(explored.begin(), explored.end(),
			make_tuple(x, y)) != explored.end())
			continue;

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
				make_tuple(x, y + 1)) == explored.end()
				&& image.GetCoordinate(x, y + 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y + 1));
		}
	}

	return explored;
}

Feature FeatureFinder::DefineFeature(const YUVImage & image,
	Rectangle region, tuple<int, int> start)
{
	vector<tuple<int, int>> pixels = GetFeaturePixels(image, region, start);
	float avgX = 0, avgY = 0, count = 0;
	for (tuple<int, int> i : pixels)
	{
		avgX += get<0>(i);
		avgY += get<1>(i);
		count++;
	}

	avgX /= count;
	avgY /= count;
	float size = sqrtf(count / (float) M_PI);
	return Feature(avgX, avgY, size);
}

vector<Feature> FeatureFinder::FindFeatures(
	const YUVImage & image, Rectangle region)
{
	vector<Feature> found;
	bool * checked = (bool *) calloc(region.width * region.height, sizeof(bool));

	// Search region for features
	for (int y = region.y; y < region.height + region.y; y++)
	{
		for (int x = region.x; x < region.width + region.x; x++)
		{
			if (image.GetCoordinate(x, y).y >= intensityThreshold)
			{
				if (!checked[x + y * region.width])
				{
					Feature m = DefineFeature(image, region, make_tuple(x, y));
					found.push_back(m);
					FillChecked(image, checked, region, make_tuple(x, y));
				}
			}
		}
	}

	free(checked);
	return found;
}
