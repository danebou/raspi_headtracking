/*
	
*/
#pragma once

class Quarternion
{
public:
	float r, i, j, k;

	Quarternion();
	Quarternion(float r, float i, float j, float k);

	bool operator==(const Quarternion& rhs) const;
	bool operator!=(const Quarternion& rhs) const;
	Quarternion operator +(const Quarternion other) const;
	Quarternion operator +(const float other) const;
	Quarternion operator *(const Quarternion other) const;
	Quarternion operator *(const float other) const;
	Quarternion operator /(const float other) const;
	Quarternion operator ~() const;
};

