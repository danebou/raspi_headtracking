/*
	A 3D position and rotation
*/
#pragma once
#include "utility\Vector3f.h"
#include "utility\Quarternion.h"

class Transformation
{
public:
	Vector3f loc;
	Quarternion rot;
	Transformation(Vector3f loc, Quarternion rot);
};
