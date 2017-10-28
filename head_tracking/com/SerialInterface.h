#pragma once
class SerialInterface
{
public:
	virtual int Read(char dst[], int length)=0;
	virtual void Write(const char src[], int length)=0;
};

