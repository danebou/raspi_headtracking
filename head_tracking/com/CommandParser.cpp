#include "CommandParser.h"
#include "com/COBS.h"
#include <string.h>

#define ToFloat(x) *((float *)&(x))

CommandParser::CommandParser(ControllerInterface& controller, SerialInterface& serial)
	: controller(controller), serial(serial)
{
	// Begin receiveCommand thread
	receiveThread = new std::thread(&CommandParser::ReceiveTask, this);
}

CommandParser::~CommandParser()
{
	delete receiveThread;
}

CommandParser::CommandError CommandParser::ParseCommand(uint8_t data[], int dataLength)
{
	if (!COBS::Decode(data, dataLength))
		return CommandError::COBS;

	// Remove leading byte in COBS
	data += 1;
	dataLength -= 1;

	if (dataLength < 1)
		return CommandError::UnexpectedLength;

	// Find type byte
	CommandType type = (CommandType)data[0];

	// Remove type from command data
	data += 1;
	dataLength -= 1;

	return DoCommand(type, data, dataLength);
}

CommandParser::CommandError CommandParser::DoCommand(CommandType type, const uint8_t data[], int dataLength)
{
	switch (type)
	{
	case CommandType::ResetCalibration:
		ResetCalibration(data, dataLength);
		break;

	case CommandType::AddCalibration:
		AddCalibration(data, dataLength);
		break;

	case CommandType::SetEyeDistance:
		break;

	default:
		return CommandError::UnknownCommand;
	}

	return CommandError::NoError;
}

CommandParser::CommandError CommandParser::AddCalibration(const uint8_t data[], int dataLength)
{
	if (dataLength != 33)
		return CommandError::UnexpectedLength;

	int cameraNum = data[0];
	Transformation t(
		Vector3f(
			ToFloat(data[1]),
			ToFloat(data[5]),
			ToFloat(data[9])
		),
		Quarternion(
			ToFloat(data[13]),
			ToFloat(data[17]),
			ToFloat(data[21]),
			ToFloat(data[25])
		)
	);
	float checkerSize = ToFloat(data[29]);

	controller.AddCalibration(cameraNum, t, checkerSize);

	return CommandError::NoError;
}

CommandParser::CommandError CommandParser::ResetCalibration(const uint8_t data[], int dataLength)
{
	(void) data; // We are not using this arg
	if (dataLength != 1)
		return CommandError::UnexpectedLength;

	//int cameraNum = data[0];

	return CommandError::NoError;
}

void CommandParser::UpdatePosition(Vector3f position)
{
	float data[6];
	data[0] = position.x;
	data[1] = position.y;
	data[2] = position.z;
	data[3] = position.x;
	data[4] = position.y;
	data[5] = position.z;

	SendCommand(CommandType::PositionData, (uint8_t*)data, sizeof(data));
}

void CommandParser::SendCommand(CommandType type, const uint8_t data[], int dataLength)
{
	uint8_t * cmdBytes = new uint8_t[dataLength + 3]; // Leave space for COBS + Type + Delimiter

													  // Concatenate Data + Type
	cmdBytes[1] = (uint8_t)type;  // Leave space for COBS
	memcpy(cmdBytes + 2, data, dataLength);
	dataLength += 1; // Add 1 to dataLength for type

	COBS::Encode(cmdBytes + 1, dataLength);
	dataLength += 1; // Add 1 to dataLength for COBS

					 // Add delimiter
	cmdBytes[dataLength] = 0;
	dataLength += 1;

	// Send command
	serial.Write((const char *)cmdBytes, dataLength);

	delete[] cmdBytes;
}
#include <iostream>
void CommandParser::ReceiveTask()
{
	while (true)
	{
		using namespace std::this_thread;
		sleep_for(chrono::milliseconds(4000));
		cout << "\n" << "Hi from unimplemented read thread!\n" << 0;
	}
}