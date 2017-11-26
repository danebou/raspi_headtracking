#include "MockSerial.h"

MockSerial::MockSerial()
{
}

int MockSerial::Read(char dst[], int length)
{
	return 0;
}

void MockSerial::Write(const char src[], int length)
{
}
