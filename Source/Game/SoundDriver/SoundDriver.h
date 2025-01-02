#ifndef INCLUDE_SOUNDDRIVER_H
#define INCLUDE_SOUNDDRIVER_H

#include "Util/Types.h"

#define PLAYBACKCONTROL_AT_REST (1 << 1)
#define PLAYBACKCONTROL_SFX_OVERRIDE (1 << 2)
#define PLAYBACKCONTROL_MODULATION (1 << 3)
#define PLAYBACKCONTROL_DO_NOT_ATTACK_NEXT (1 << 4)
#define PLAYBACKCONTROL_PLAYING (1 << 7)

#define VOICECONTROL_IS_FM2 (1 << 2)
#define VOICECONTROL_IS_PSG (1 << 7)

typedef struct
{
	u8 PlaybackControl;
	u8 VoiceControl;

	u8 TempoDivider;

	u8 pad1;

	u8 *DataPointer;

	union
	{
		struct
		{
			u8 Transpose;
			u8 Volume;
			u8 AMSFMSPan;
			u8 VoiceIndex;
			u8 _VolEnvIndex;
			u8 StackPointer;
			u8 DurationTimeout;
			u8 SavedDuration;
			s16 Freq;
			u8 NoteTimeout;
			u8 NoteTimeoutMaster;
			u8 *ModulationPtr;
			u8 ModulationWait;
			u8 ModulationSpeed;
			u8 ModulationDelta;
			u8 ModulationSteps;
			u16 ModulationVal;
			u8 Detune;
			u8 FeedbackAlgo;
			u8 *VoicePtr;
		} fm;
		struct
		{
			u8 Transpose;
			u8 Volume;
			u8 _AMSFMSPan;
			u8 VoiceIndex;
			u8 VolEnvIndex;
			u8 StackPointer;
			u8 DurationTimeout;
			u8 SavedDuration;
			s16 Freq;
			u8 NoteTimeout;
			u8 NoteTimeoutMaster;
			u8 *ModulationPtr;
			u8 ModulationWait;
			u8 ModulationSpeed;
			u8 ModulationDelta;
			u8 ModulationSteps;
			u16 ModulationVal;
			u8 Detune;
			u8 PSGNoise;
			u8 *_VoicePtr;
		} psg;
		struct
		{
			u8 _Transpose;
			u8 _Volume;
			u8 AMSFMSPan;
			u8 _VoiceIndex;
			u8 _VolEnvIndex;
			u8 StackPointer;
			u8 DurationTimeout;
			u8 SavedDuration;
			u8 SavedDAC;
			u8 pad1; /* 2nd byte of Freq */
			u8 _NoteTimeout;
			u8 _NoteTimeoutMaster;
			u8 *_ModulationPtr;
			u8 _ModulationWait;
			u8 _ModulationSpeed;
			u8 _ModulationDelta;
			u8 _ModulationSteps;
			u16 _ModulationVal;
			u8 _Detune;
			u8 _FeedbackAlgo;
			u8 *_VoicePtr;
		} dac;
	} u;
	union
	{
		u8 LoopCounters[sizeof(u8*) * 3];
		u8 *Stack[3];
	} top;
} SoundDriverTrack;

typedef struct
{
	u8 v_sndprio;
	u8 v_main_tempo_timeout;
	u8 v_main_tempo;
	u8 f_pausemusic;
	u8 v_fadeout_counter;
	u8 pad1;
	u8 v_fadeout_delay;
	u8 v_communication_byte;
	u8 f_updating_dac;
	u8 v_sound_id;
	u8 v_soundqueue[3];
	u8 pad2;
	u8 f_voice_selector;
	u8 pad3[9];
	u8 *v_voice_ptr;
	u8 pad4[4];
	u8 *v_special_voice_ptr;
	u8 f_fadein_flag;
	u8 v_fadein_delay;
	u8 v_fadein_counter;
	u8 f_1up_playing;
	u8 v_tempo_mod;
	u8 v_speeduptempo;
	u8 f_speedup;
	u8 v_ring_speaker;
	u8 f_push_playing;
	u8 pad5[0x13];

	SoundDriverTrack v_music_dac_track[1];
	SoundDriverTrack v_music_fm1_track[6];
	SoundDriverTrack v_music_psg1_track[3];
} SoundDriverState;

typedef struct
{
	SoundDriverState state;

	SoundDriverTrack v_sfx_fm3_track[3];
	SoundDriverTrack v_sfx_psg1_track[3];

	SoundDriverTrack v_spcsfx_fm4_track[1];
	SoundDriverTrack v_spcsfx_psg3_track[1];

	SoundDriverState state_1up;
} SoundDriverRAM;

extern SoundDriverRAM v_snddriver_ram;

void UpdateMusic(void);

#endif
