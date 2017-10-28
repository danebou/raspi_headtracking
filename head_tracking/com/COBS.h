#pragma once
#include <stdint.h>

class COBS
{
public:
	static void Encode(uint8_t data[], int dataLength);
	static bool Decode(uint8_t data[], int dataLength);
};

