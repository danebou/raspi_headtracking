#ifndef VECTOR3F_H
#define VECTOR3F_H

struct Vector3f
{
    float x, y, z;

    Vector3f();
    Vector3f(float x, float y, float z);

	Vector3f operator+(const Vector3f&) const;
	bool operator==(const Vector3f& rhs) const;
	bool operator!=(const Vector3f& rhs) const;
};

#endif /* VECTOR3F_H */