#include "RpiSerial.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>
#include <string.h>

int RpiSerial::Init() 
{
    // Open serial port (USBOtg is "ttyGSO")
    sfd = open("/dev/ttyGS0", O_RDWR | O_NOCTTY);
    if (tcgetattr(sfd, &options) < 0)
        return -1;

    // Configure baud rate and options
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
    options.c_cflag &= ~(CSIZE | PARENB);
    options.c_cflag |= CS8;
    options.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
        INLCR | PARMRK | INPCK | ISTRIP | IXON);
    options.c_oflag = 0;
    if (cfsetospeed(&options, B115200) < 0
    || cfsetispeed(&options, B115200) < 0)
        return -1;

    // Set options
    //cfmakeraw(&options);
    if (tcsetattr(sfd, TCSANOW, &options) < 0)
        return -1;

    return sfd;
}

RpiSerial::RpiSerial()
{
	if (Init() == -1) {
        printf("Err no: %d\n", errno);
        printf("Err description:\n %s\n", strerror(errno));
		throw "Error: failed to open serial";
    }
}

RpiSerial::~RpiSerial()
{
	close(sfd);
}

int RpiSerial::Read(char dst[], int length)
{
	return read(sfd, dst, length);
}

void RpiSerial::Write(const char src[], int length)
{
	/*std::cout << "Write data(" << length << "): ";
	for (int i = 0; i < length; i++)
	{
		std::cout << " 0x" << std::hex << (int)((uint8_t *) src)[i] << std::dec;
	}
	std::cout << "\n";*/
	write(sfd, src, length);
}
