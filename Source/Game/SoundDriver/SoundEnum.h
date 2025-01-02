#ifndef INCLUDE_SOUNDENUM_H
#define INCLUDE_SOUNDENUM_H

enum
{
	bgm__First = 0x81,
	bgm_GHZ = 0x81,
	bgm_LZ,
	bgm_MZ,
	bgm_SLZ,
	bgm_SYZ,
	bgm_SBZ,
	bgm_Invincible,
	bgm_ExtraLife,
	bgm_SS,
	bgm_Title,
	bgm_Ending,
	bgm_Boss,
	bgm_FZ,
	bgm_GotThrough,
	bgm_GameOver,
	bgm_Continue,
	bgm_Credits,
	bgm_Drowning,
	bgm_Emerald,
	bgm__Last,

	sfx__First = 0xA0,
	sfx_Jump = 0xA0,
	sfx_Lamppost,
	sfx_A2,
	sfx_Death,
	sfx_Skid,
	sfx_A5,
	sfx_HitSpikes,
	sfx_Push,
	sfx_SSGoal,
	sfx_SSItem,
	sfx_Splash,
	sfx_AB,
	sfx_HitBoss,
	sfx_Bubble,
	sfx_Fireball,
	sfx_Shield,
	sfx_Saw,
	sfx_Electric,
	sfx_Drown,
	sfx_Flamethrower,
	sfx_Bumper,
	sfx_Ring,
	sfx_SpikesMove,
	sfx_Rumbling,
	sfx_B8,
	sfx_Collapse,
	sfx_SSGlass,
	sfx_Door,
	sfx_Teleport,
	sfx_ChainStomp,
	sfx_Roll,
	sfx_Continue,
	sfx_Basaran,
	sfx_BreakItem,
	sfx_Warning,
	sfx_GiantRing,
	sfx_Bomb,
	sfx_Cash,
	sfx_RingLoss,
	sfx_ChainRise,
	sfx_Burning,
	sfx_Bonus,
	sfx_EnterSS,
	sfx_WallSmash,
	sfx_Spring,
	sfx_Switch,
	sfx_RingLeft,
	sfx_Signpost,
	sfx__Last,

	spec__First = 0xD0,
	spec_Waterfall = 0xD0,
	spec__Last,

	flg__First = 0xE0,
	bgm_Fade = 0xE0,
	sfx_Sega,
	bgm_Speedup,
	bgm_Slowdown,
	bgm_Stop,
	flg__Last
};

#endif
