#include "PSG/PSG.h"

#include <stdio.h>

#include <Windows.h>

void PSG_Input(u8 data)
{
	(void)data;

	char buffer[256];
	sprintf(buffer, "PSG_Input: data = %02X\n", data);
	OutputDebugStringA(buffer);
}
