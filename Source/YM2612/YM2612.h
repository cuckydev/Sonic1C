#ifndef INCLUDE_YM2612_H
#define INCLUDE_YM2612_H

#include "Util/Types.h"

void YM2612_Init(void);

void YM2612_InputA(u8 addr, u8 data);
void YM2612_InputB(u8 addr, u8 data);

#endif
