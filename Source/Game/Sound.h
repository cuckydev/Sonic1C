#ifndef INCLUDE_SOUND_H
#define INCLUDE_SOUND_H

#include "Util/Types.h"

void DACDriverLoad(void);

void PlaySound(u8 code);
void PlaySound_Special(u8 code);
void PlaySound_Unused(u8 code);

#endif
