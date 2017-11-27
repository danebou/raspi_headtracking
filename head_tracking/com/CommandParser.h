#pragma once
#include "ControllerInterface.h"
#include <thread>
#include <mutex>

using namespace std;

class ControllerInterface;

class CommandParser
{
public:
	enum class CommandError {
		NoError = 0,
		UnexpectedLength = -1,
		COBS = -2,
		UnknownCommand = -3
	};
	enum class CommandType {
		ResetCalibration = 0x01,
		AddCalibration = 0x02,
		SetEyeDistance = 0x04,
		OutputConsole = 0x05,
		PositionData = 0x06,
	};

	CommandParser(ControllerInterface& controller, SerialInterface& serial);
	~CommandParser();
	CommandError ParseCommand(uint8_t data[], int dataLength);
	void SendCommand(CommandType type, const uint8_t data[], int dataLength);
	void UpdatePosition(Vector3f position);

private:
	CommandError DoCommand(CommandType type, const uint8_t data[], int dataLength);
	CommandError ResetCalibration(const uint8_t data[], int dataLength);
	CommandError AddCalibration(const uint8_t data[], int dataLength);
	void ReceiveTask();

	thread * receiveThread;
	ControllerInterface& controller;
	SerialInterface& serial;
	mutex stopThread_m;
	bool stopThread = false;
};

