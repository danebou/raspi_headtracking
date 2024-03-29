#include "Rectangle.h"

Rectangle::Rectangle()
{
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

Rectangle::Rectangle(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height)
{
}

bool Rectangle::operator==(const Rectangle& rhs) const
{
    return (x == rhs.x 
        && y == rhs.y
        && width == rhs.width
        && height == rhs.height);
}

bool Rectangle::operator!=(const Rectangle& rhs) const
{
    return !(*this == rhs);
}