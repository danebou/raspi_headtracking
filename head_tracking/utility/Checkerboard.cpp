#include "Checkerboard.h"

using namespace cv;

Checkerboard::Checkerboard(float size, int rows, int cols)
	: size(size), rows(rows), cols(cols)
{
	// Initialize array to flat checkerboard pattern
	// There are (rows + 1) vertical points on a checkerboard
	corners.resize(rows + 1);
	for (int y = 0; y <= rows; y++)
	{
		// There are (cols + 1) horizontal points on a checkerboard
		corners[y].resize(cols + 1);
		for (int x = 0; x <= cols; x++)
		{
			corners[y][x] = Vector3f(x * size, y * size, 0);
		}
	}
}

Checkerboard::Checkerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols)
	: Checkerboard(checkerSize, checkerRows, checkerCols)
{
	ApplyTransformation(t);
}

void Checkerboard::ApplyTransformation(Transformation t)
{
	// Don't do anything if we don't have corners
	if (corners.size() <= 0)
		return;

	// Apply transformation
	for (int y = 0; y < rows + 1; y++)
	{
		for (int x = 0; x < cols + 1; x++)
		{
			Vector3f pos = corners[y][x];

			// Convert to quaternion point
			Quarternion point = Quarternion(0, pos.x, pos.y, pos.z);

			// Quaternion rotation magic (qpq^-1)
			Quarternion rotatedPoint = t.rot * point * (~t.rot);

			// Add position transformation
			corners[y][x] = Vector3f(rotatedPoint.i, rotatedPoint.j, rotatedPoint.k) + t.loc;
		}
	}
}

vector<Point3f> Checkerboard::GetInnerCorners()
{
	vector<Point3f> inner = vector<Point3f>((rows - 1) * (cols - 1));
	for (int y = 1; y < rows; y++)
	{
		for (int x = 1; x < cols; x++)
		{
			Vector3f v = corners[y][x];
			inner[(x - 1) + (y - 1) * (cols - 1)] = Point3f(v.x, v.y, v.z);
		}
	}
	return inner;
}
