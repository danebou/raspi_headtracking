/*
	Solves the PnP problem to find the pose of a rigid body
*/
#pragma once
#include "utility/Vector3f.h"
#include <vector>

using namespace std;

class PnPSolver
{
public:
	PnPSolver(vector<Vector3f> objectCoordinates);
	//Solve(vector<Vector3f>);
private:
	vector<Vector3f> objectCoordinates;
};

