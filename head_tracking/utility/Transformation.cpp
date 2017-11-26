#include "Transformation.h"

Transformation::Transformation()
{
	rot = Quarternion(1, 0, 0, 0); // No rotation
}

Transformation::Transformation(Vector3f loc, Quarternion rot)
	: loc(loc), rot(rot)
{
}
