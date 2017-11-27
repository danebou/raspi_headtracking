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
	int i = 0;
	for (; i < maxIterations && !locations.empty(); i++)
	{
		tuple<int, int> next = locations.front();
		int x = get<0>(next);
		int y = get<1>(next);
		locations.pop();

		if (checkarray[(x - region.x) + (y - region.y) * region.width])
			continue;

		if (image.GetCoordinate(x, y).y >= intensityThreshold)
		{
			checkarray[(x - region.x) + (y - region.y) * region.width] = true;
		}
		else
		{
			continue;
		}

		if (x - 1 >= region.x)
		{
			if (!checkarray[(x - 1 - region.x) + (y - region.y) * region.width]
				&& image.GetCoordinate(x - 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x - 1, y));
		}
		if (x + 1 < region.x + region.width)
		{
			if (!checkarray[(x + 1 - region.x) + (y - region.y) * region.width]
				&& image.GetCoordinate(x + 1, y).y >= intensityThreshold)
				locations.push(make_tuple(x + 1, y));
		}
		if (y - 1 >= region.y)
		{
			if (!checkarray[(x - region.x) + (y - 1 - region.y) * region.width]
				&& image.GetCoordinate(x, y - 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y - 1));
		}
		if (y + 1 < region.y + region.height)
		{
			if (!checkarray[(x - region.x) + (y + 1 - region.y) * region.width]
				&& image.GetCoordinate(x, y + 1).y >= intensityThreshold)
				locations.push(make_tuple(x, y + 1));
		}
	}

	if (i >= maxIterations)
		reachedMaxIter = true;
}

vector<tuple<int, int>> FeatureFinder::GetFeaturePixels(const YUVImage & image,
	Rectangle region, tuple<int, int> start)
{
	// This function just fills in the rest, it will be optimized out later
	queue<tuple<int, int>> locations;
	vector<tuple<int, int>> explored;

	locations.push(start);
	int i = 0;
	for (; i < maxIterations && !locations.empty(); i++)
	{
		tuple<int, int> next = locations.front();
		int x = get<0>(next);
		int y = get<1>(next);
		locations.pop();

		if (locations.size() > 2000)
		{
			int t = -0;
		}

		if (find(explored.begin(), explored.end(),
			make_tuple(x, y)) != explored.end())
			continue;

		if (image.GetCoordinate(x, y).y >= intensityThreshold)
		{
			explored.push_back(make_tuple(x, y));
		}
		else
		{
			continue;
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

	if (i >= maxIterations)
		reachedMaxIter = true;

	return explored;
}

bool FeatureFinder::DefineFeature(const YUVImage & image,
	Rectangle region, tuple<int, int> start, Feature * feature)
{
	vector<tuple<int, int>> pixels = GetFeaturePixels(image, region, start);

	if (reachedMaxIter)
		return false;

	float avgX = 0, avgY = 0, count = 0;
	for (tuple<int, int> i : pixels)
	{
		int x = get<0>(i);
		int y = get<1>(i);
		if (x == region.x || y == region.y
			|| x == region.x + region.width - 1
			|| y == region.y + region.height - 1)
		{
			return false;
		}
		avgX += x;
		avgY += y;
		count++;
	}

	avgX /= count;
	avgY /= count;
	float size = sqrtf(count / (float) M_PI);
	*feature = Feature(avgX, avgY, size);
	return true;
}

vector<Feature> FeatureFinder::FindFeatures(
	const YUVImage & image, Rectangle region)
{
	vector<Feature> found;
	bool * checked = (bool *) calloc(region.width * region.height, sizeof(bool));

	reachedMaxIter = false;

	// Search region for features
	for (int y = region.y; y < region.height + region.y; y++)
	{
		for (int x = region.x; x < region.width + region.x; x++)
		{
			if (image.GetCoordinate(x, y).y >= intensityThreshold)
			{
				if (!checked[(x - region.x) + (y - region.y) * region.width])
				{
					Feature m(0, 0, 0);
					if (DefineFeature(image, region, make_tuple(x, y), &m))
						found.push_back(m);
					FillChecked(image, checked, region, make_tuple(x, y));

					// Max iterations reached, none found
					if (reachedMaxIter)
						return vector<Feature>();
				}
			}
		}
	}

	free(checked);
	return found;
}
