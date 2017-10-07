#include "vector3f.h"

Vector3f::Vector3f()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3f::Vector3f(float x, float y, float z)
    : x(x), y(y), z(z)
{
}

bool Vector3f::operator==(const Vector3f& other) const
{
	return (x == other.x) 
		&& (y == other.y)
		&& (z == other.z);
}

bool Vector3f::operator!=(const Vector3f& other) const
{
	return !(*this == other);
}

Vector3f Vector3f::operator+(const Vector3f& other) const
{
	return Vector3f(
		x + other.x,
		y + other.y,
		z + other.z
	);
}