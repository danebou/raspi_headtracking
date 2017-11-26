#ifndef RECTANGLE_H
#define RECTANGLE_H

struct Rectangle
{
public:
    int x, y;
    int width, height;

    Rectangle();
    Rectangle(int x, int y, int width, int height);
    bool operator==(const Rectangle& rhs) const;
	bool operator!=(const Rectangle& rhs) const;
};

#endif /* RECTANGLE_H */