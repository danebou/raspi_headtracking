#pragma once
#include "com\SerialInterface.h"
class MockSerial :
	public SerialInterface
{
public:
	MockSerial();
	int Read(char dst[], int length);
	void Write(const char src[], int length);
};

