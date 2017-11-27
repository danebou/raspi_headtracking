#include "CommandParser.h"
#include "com/COBS.h"
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

CommandParser::CommandParser(ControllerInterface& controller, SerialInterface& serial)
	: controller(controller), serial(serial)
{
	receiveThread = new thread(&CommandParser::ReceiveTask, this);
}

CommandParser::~CommandParser()
{

	{
		lock_guard<mutex> lg(stopThread_m);
		stopThread = true;
	}

	// Wait for threads to finish canceling 
	// We could in theory just call delete, but this is causing gtest to freak out. (But why??)
	if (receiveThread->joinable())
		receiveThread->join();

	delete receiveThread;
}

CommandParser::CommandError CommandParser::ParseCommand(uint8_t data[], int dataLength)
{
	if (!COBS::Decode(data, dataLength)) {
		return CommandError::COBS;
	}

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
		return ResetCalibration(data, dataLength);

	case CommandType::AddCalibration:
		return AddCalibration(data, dataLength);

	case CommandType::SetEyeDistance:
		break;

	default:
		return CommandError::UnknownCommand;
	}

	return CommandError::NoError;
}

CommandParser::CommandError CommandParser::AddCalibration(const uint8_t data[], int dataLength)
{
	if (dataLength != 40)
		return CommandError::UnexpectedLength;

	Transformation t = Transformation(Vector3f(), Quarternion());
	memcpy(&t.loc.x, &data[0], 4);
	memcpy(&t.loc.y, &data[4], 4);
	memcpy(&t.loc.z, &data[8], 4);
	memcpy(&t.rot.r, &data[12], 4);
	memcpy(&t.rot.i, &data[16], 4);
	memcpy(&t.rot.j, &data[20], 4);
	memcpy(&t.rot.k, &data[24], 4);

	float checkerSize;
	int checkerRows, checkerCol;
	memcpy(&checkerSize, &data[28], 4);
	memcpy(&checkerRows, &data[32], 4);
	memcpy(&checkerCol, &data[36], 4);

	controller.AddCalibration(0, t, checkerSize, checkerRows, checkerCol);

	return CommandError::NoError;
}

CommandParser::CommandError CommandParser::ResetCalibration(const uint8_t data[], int dataLength)
{
	(void) data; // We are not using this arg
	if (dataLength != 0)
		return CommandError::UnexpectedLength;

	//int cameraNum = data[0];
	controller.ResetCalibration(0);

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

void CommandParser::ReceiveTask()
{
	static char readChar;
	vector<char> buffer;

	while (true)
	{
		{
			lock_guard<mutex> lg(stopThread_m);
			if (stopThread)
				break;
		}
		using namespace std::this_thread;
		sleep_for(chrono::milliseconds(1000));

		while (serial.Read(&readChar, 1) > 0)
		{
			if (readChar == 0x00) {
				CommandError err = ParseCommand(reinterpret_cast<uint8_t *>(buffer.data()), (int) buffer.size());
				if (err != CommandError::NoError)
				{
					cout << "Failed to parse a command (Err: " << (int) err <<")\n"; 
				}

				buffer.clear();
				continue;
			}

			buffer.push_back(readChar);
		}
	}
}