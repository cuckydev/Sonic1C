#include "Game/Sound.h"

#include "Game/SoundDriver/SoundDriver.h"

void DACDriverLoad(void)
{

}

void PlaySound(u8 code)
{
	v_snddriver_ram.state.v_soundqueue[0] = code;
}

void PlaySound_Special(u8 code)
{
	v_snddriver_ram.state.v_soundqueue[1] = code;
}

void PlaySound_Unused(u8 code)
{
	v_snddriver_ram.state.v_soundqueue[2] = code;
}
