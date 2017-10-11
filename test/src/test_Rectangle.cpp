#include <gtest\gtest.h>
#include "utility\Rectangle.h"

// Tests initialization of a vector using the default constructor
TEST(Rectangle, HandlesZeroInput)
{
	Rectangle r;
	EXPECT_EQ(0, r.x);
	EXPECT_EQ(0, r.y);
	EXPECT_EQ(0, r.width);
	EXPECT_EQ(0, r.height);
}

// Tests initialization of a vector using a 
TEST(Rectangle, HandlesInitInput)
{
	Rectangle r(1, 2, 3, 4);
	EXPECT_EQ(1, r.x);
	EXPECT_EQ(2, r.y);
	EXPECT_EQ(3, r.width);
	EXPECT_EQ(4, r.height);
}