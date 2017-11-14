#include "Quarternion.h"

Quarternion::Quarternion()
{
	r = 0;
	i = 0;
	j = 0;
	k = 0;
}

Quarternion::Quarternion(float r, float i, float j, float k)
	: r(r), i(i), j(j), k(k)
{
}

bool Quarternion::operator==(const Quarternion & rhs) const
{
	return (
		this->r == rhs.r
		&& this->i == rhs.i
		&& this->j == rhs.j
		&& this->k == rhs.k
	);
}

bool Quarternion::operator!=(const Quarternion & rhs) const
{
	return !(*this == rhs);
}

Quarternion Quarternion::operator+(const Quarternion other) const
{
	return Quarternion(
		this->r + other.r,
		this->i + other.i,
		this->j + other.j,
		this->k + other.k
	);
}

Quarternion Quarternion::operator+(const float other) const
{
	return Quarternion(this->r + other, this->i, this->j, this->k);
}

Quarternion Quarternion::operator*(const Quarternion other) const
{
	return Quarternion(
		this->r * other.r - this->i * other.i - this->j * other.j - this->k * other.k,
		this->r * other.i + this->i * other.r + this->j * other.k - this->k * other.j,
		this->r * other.j - this->i * other.k + this->j * other.r + this->k * other.i,
		this->r * other.k + this->i * other.j - this->j * other.i + this->k * other.r
	);
}

Quarternion Quarternion::operator*(const float other) const
{
	return Quarternion(
		this->r * other, 
		this->i * other, 
		this->j * other, 
		this->k * other
	);
}

Quarternion Quarternion::operator/(const float other) const
{
	return Quarternion(
		this->r / other,
		this->i / other,
		this->j / other,
		this->k / other
	);
}

Quarternion Quarternion::operator ~() const
{
	Quarternion(
		this->r,
		-this->i,
		-this->j,
		-this->k
	);
}
