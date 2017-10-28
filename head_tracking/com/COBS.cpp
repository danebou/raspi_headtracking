#include "com/COBS.h"

void COBS::Encode(uint8_t data[], int dataLength)
{
	int nextZero = 1;
	for (int i = dataLength - 1; i >= 0; i--, nextZero++)
	{
		// Look for zeros
		if (data[i] != 0)
		{
			continue;
		}

		// Zero was found

		// Replace zero with position to next
		data[i] = nextZero;

		// Reset the counter to the nextZero
		nextZero = 0;
	}
	
	// Set the first byte
	data[-1] = nextZero;
}

bool COBS::Decode(uint8_t data[], int dataLength)
{
	int index = data[0];
	while (index < dataLength)
	{
		int nextIndex = data[index];

		// Avoiding looping forever, make sure we are always moving forward
		if (nextIndex <= 0)
			return false; 

		// Get next index and replace byte
		data[index] = 0;
		index += nextIndex;
	}

	// Verify index lies at end
	return index == dataLength;
}
