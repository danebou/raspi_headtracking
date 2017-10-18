/*
	A feature point of marker. This as a group of "similar" pixels that have a 
	2D coordinate position property and a size.
*/
#ifndef FEATURE_H
#define FEATURE_H

class Feature
{
public:
	float x, y;
	float size;

	Feature(float x, float y, float size);
	inline bool operator==(const Feature& rhs) const
	{
		return (x == rhs.x) && (y == rhs.y) && (size == rhs.size);
	}
};

#endif /* FEATURE_H */