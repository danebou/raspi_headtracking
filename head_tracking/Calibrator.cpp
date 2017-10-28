#include "Calibrator.h"
#include <iostream>

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

void Calibrator::Add(Transformation t, float checkerSize)
{
	cout << "Added calibration (" << count << "):\n";
	cout << "	Checkerboard size = " << checkerSize << "\n";
	cout << "	Position: (" << t.loc.x << ", " << t.loc.y << ", " << t.loc.z << ")\n";
	cout << "   Quaternion: (" << t.rot.r << ", " << t.rot.i << ", " << t.rot.j << ", " << t.rot.k << ")\n";
	isCalibrated = true;
	count++;
}
