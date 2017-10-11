#ifndef RECTANGLE_H
#define RECTANGLE_H

struct Rectangle
{
public:
    int x, y;
    int width, height;

    Rectangle();
    Rectangle(int x, int y, int width, int height);
};

#endif /* RECTANGLE_H */