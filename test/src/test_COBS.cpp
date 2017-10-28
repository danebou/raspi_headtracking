#include <gtest\gtest.h>
#include "com\COBS.h"

TEST(COBS, Encode1)
{
	char decoded[] = { 0 };
	char encoded[] = { 1, 1 };
	char working[sizeof(decoded) + 1];

	memcpy(working + 1, decoded, sizeof(decoded));


}