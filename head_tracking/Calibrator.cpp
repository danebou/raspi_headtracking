#include "Calibrator.h"
#include "utility/Quarternion.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;

Calibrator::Calibrator()
{
	isCalibrated = false;
	count = 0;
}

void Calibrator::Reset()
{
	cout << "Reset calibration\n";
	isCalibrated = false;
	count = 0;
}

void Calibrator::Add(Transformation t, float checkerSize, int checkerRows, int checkerCols)
{
	cout << "Added calibration (" << count << "):\n";
	cout << "   Checkerboard: " << checkerRows << "x" << checkerCols << "\n";
	cout << "	Checkerboard size: " << checkerSize << "\n";
	cout << "	Position: (" << t.loc.x << ", " << t.loc.y << ", " << t.loc.z << ")\n";
	cout << "   Quaternion: (" << t.rot.r << ", " << t.rot.i << ", " << t.rot.j << ", " << t.rot.k << ")\n";
	isCalibrated = true;

	count++;
}

vector<vector<Vector3f>> Calibrator::GenerateCheckerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols) 
{
	vector<vector<Vector3f>> checkerboard;

	// Initialize array to flat checkerboard pattern
	checkerboard.resize(checkerRows);
	for (int y = 0; y < checkerRows; y++)
	{
		checkerboard[y].resize(checkerCols);
		for (int x = 0; x < checkerCols; x++)
		{
			checkerboard[y][x] = Vector3f(x * checkerSize, y * checkerSize, 0);
		}
	}

	// Apply transformation
	for (int y = 0; y < checkerRows; y++)
	{
		for (int x = 0; x < checkerCols; x++)
		{
			Vector3f pos = checkerboard[y][x];
			
			// Convert to quaternion point
			Quarternion point = Quarternion(0, pos.x, pos.y, pos.z);
			
			// Quaternion rotation magic (qpq^-1)
			Quarternion rotatedPoint = t.rot * point * (~t.rot);
			
			// Add position transformation
			checkerboard[y][x] = Vector3f(rotatedPoint.i, rotatedPoint.j, rotatedPoint.k) + t.loc;
		}
	}

	return checkerboard;
}