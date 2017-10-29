#pragma once
#include "SerialInterface.h"
#include <termios.h>

class RpiSerial : public SerialInterface
{
public:
	RpiSerial();
	~RpiSerial();
	int Read(char dst[], int length);
	void Write(const char src[], int length);

private:

	int Init();

	int sfd;
	struct termios options;
};

