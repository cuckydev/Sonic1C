#include "YM2612/YM2612.h"

#include <Windows.h>

#include <stdio.h>

void YM2612_Init(void)
{

}

void YM2612_Input1(u8 addr, u8 data)
{
	(void)addr;
	(void)data;

	char buffer[256];
	sprintf(buffer, "YM2612_Input1: addr = %02X, data = %02X\n", addr, data);
	OutputDebugStringA(buffer);
}

void YM2612_Input2(u8 addr, u8 data)
{
	(void)addr;
	(void)data;

	char buffer[256];
	sprintf(buffer, "YM2612_Input2: addr = %02X, data = %02X\n", addr, data);
	OutputDebugStringA(buffer);
}
