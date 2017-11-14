#include "Calibrator.h"
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

vector<vector<Vector3>> Calibrator::GenerateCheckerboard(Transformation t, float checkerSize, int checkerRows, int checkerCols) 
{
	vector<vector<Vector3>> checkboard;

	// Initialize array to flat checkerboard pattern
	checkboard.resize(checkerRows);
	for (int y = 0; y < checkerRows; y++)
	{
		checkboard[y].resize(checkerCols);
		for (int x = 0; x < checkerCols; x++)
		{
			checkerboard[y][x] = Vector3(x * checkerSize, y * checkerSize, 0);
		}
	}

	// Apply transformation
	for (int y = 0; y < checkerRows; y++)
	{
		for (int x = 0; x < checkerCols; x++)
		{
			Vector3 pos = checkerboard[y][x];
			
			// Convert to quaternion point
			Quaternion point = Quaternion(0, pos.x, pos.y, pos.z);
			
			// Quaternion rotation magic (qpq^-1)
			Quaternion rotatedPoint = t.rot * point * (~t.rot);
			
			// Add position transformation
			checkerboard[y][x] = Vector3(rotatedPoint.x, rotatedPoint.y, rotatedPoint.z) + t.pos;
		}
	}

	return checkboard;
}