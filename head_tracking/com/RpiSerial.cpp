#include "RpiSerial.h"
#include <iostream>

RpiSerial::RpiSerial()
{
}

RpiSerial::~RpiSerial()
{
}

int RpiSerial::Read(char dst[], int length)
{
	(void) dst;
	(void) length;
	return 0;
}

void RpiSerial::Write(const char src[], int length)
{
	std::cout << "Write data(" << length << "): ";
	for (int i = 0; i < length; i++)
	{
		std::cout << " 0x" << std::hex << (int)((uint8_t *) src)[i] << std::dec;
	}
	std::cout << "\n";
}
