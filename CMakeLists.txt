cmake_minimum_required(VERSION 3.15)

project(Sonic1C
	LANGUAGES C CXX
)

# Paths
set(AS_PATH "" CACHE FILEPATH "Path to AS assembler")
set(P2BIN_PATH "" CACHE FILEPATH "Path to p2bin tool")

# Require them to exist
if(NOT EXISTS ${AS_PATH})
	message(FATAL_ERROR "AS assembler not found at '${AS_PATH}'")
endif()
if(NOT EXISTS ${P2BIN_PATH})
	message(FATAL_ERROR "p2bin tool not found at '${P2BIN_PATH}'")
endif()

# Options
option(FIX_MAJOR_UB "Fix major undefined behavior that completely breaks things" ON)
option(FIX_BUGS "Fix bugs" OFF)

# Compiler configuration
add_library(Sonic1C.Config INTERFACE)

include(CheckIPOSupported)

check_ipo_supported(RESULT result)
if(result)
	set_property(TARGET Sonic1C.Config PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Strict configuration (Force ANSI C, enable all warnings and treat them as errors)
add_library(Sonic1C.StrictConfig INTERFACE)

target_compile_features(Sonic1C.StrictConfig INTERFACE c_std_90)

if (MSVC)
	if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		target_compile_options(Sonic1C.StrictConfig INTERFACE /Za)
	endif()
	target_compile_options(Sonic1C.StrictConfig INTERFACE /W4 /WX)
else()
	target_compile_options(Sonic1C.StrictConfig INTERFACE -ansi -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(Sonic1C.StrictConfig INTERFACE Sonic1C.Config)

# Compile the game
add_library(Sonic1C.Game STATIC
	"Source/Game/Main.c"
	"Source/Game/Sound.c"
	"Source/Game/Sound.h"
	
	"Source/Game/SoundDriver/SoundTable.c"
	"Source/Game/SoundDriver/SoundTable.h"
	"Source/Game/SoundDriver/SoundDriver.c"
	"Source/Game/SoundDriver/SoundDriver.h"
	"Source/Game/SoundDriver/SoundData.c"
	"Source/Game/SoundDriver/SoundSfxTable.c"
	"Source/Game/SoundDriver/SoundSfxTable.h"
	"Source/Game/SoundDriver/SoundSfxData.c"
)

target_include_directories(Sonic1C.Game PRIVATE "Source")
target_include_directories(Sonic1C.Game PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/Assets")

target_link_libraries(Sonic1C.Game PRIVATE Sonic1C.StrictConfig)

if (FIX_MAJOR_UB)
	target_compile_definitions(Sonic1C.Game PRIVATE SONIC1C_FIX_MAJOR_UB)
endif()
if (FIX_BUGS)
	target_compile_definitions(Sonic1C.Game PRIVATE SONIC1C_FIX_BUGS)
endif()

# Compile hardware emulation libraries
add_library(Sonic1C.Hardware STATIC
	"Source/YM2612/YM2612.c"
	"Source/YM2612/YM2612.h"

	"Source/PSG/PSG.c"
	"Source/PSG/PSG.h"
)

target_include_directories(Sonic1C.Hardware PRIVATE "Source")

target_link_libraries(Sonic1C.Hardware PRIVATE Sonic1C.Config)

# Compile the executable
add_executable(Sonic1C WIN32
	"Source/Exe/Main.c"
)

target_include_directories(Sonic1C PRIVATE "Source")

target_link_libraries(Sonic1C PRIVATE Sonic1C.Config Sonic1C.Game Sonic1C.Hardware)

# Link SDL
target_link_directories(Sonic1C PRIVATE "ThirdParty/SDL/lib/x64")
target_include_directories(Sonic1C PRIVATE "ThirdParty/SDL/include")

target_link_libraries(Sonic1C PRIVATE SDL winmm dxguid dinput8)

# Copy SDL dll to the output directory
add_custom_command(TARGET Sonic1C POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL/lib/x64/SDL.dll"
		"$<TARGET_FILE_DIR:Sonic1C>"
)

# Compile bin2h
add_subdirectory("Tools/bin2h" EXCLUDE_FROM_ALL)

# Assets
set(ASSET_BINS
	"artkos/Flowers at Ending.kos"

	"artnem/8x8 - GHZ1.nem"
	"artnem/8x8 - GHZ2.nem"
	"artnem/8x8 - LZ.nem"
	"artnem/8x8 - MZ.nem"
	"artnem/8x8 - SBZ.nem"
	"artnem/8x8 - SLZ.nem"
	"artnem/8x8 - SYZ.nem"
	"artnem/Animal Chicken.nem"
	"artnem/Animal Flicky.nem"
	"artnem/Animal Penguin.nem"
	"artnem/Animal Pig.nem"
	"artnem/Animal Rabbit.nem"
	"artnem/Animal Seal.nem"
	"artnem/Animal Squirrel.nem"
	"artnem/Boss - Eggman after FZ Fight.nem"
	"artnem/Boss - Eggman in SBZ2 & FZ.nem"
	"artnem/Boss - Exhaust Flame.nem"
	"artnem/Boss - Final Zone.nem"
	"artnem/Boss - Main.nem"
	"artnem/Boss - Weapons.nem"
	"artnem/Continue Screen Sonic.nem"
	"artnem/Continue Screen Stuff.nem"
	"artnem/Ending - Credits.nem"
	"artnem/Ending - Emeralds.nem"
	"artnem/Ending - Flowers.nem"
	"artnem/Ending - Sonic.nem"
	"artnem/Ending - StH Logo.nem"
	"artnem/Ending - Try Again.nem"
	"artnem/Enemy Ball Hog.nem"
	"artnem/Enemy Basaran.nem"
	"artnem/Enemy Bomb.nem"
	"artnem/Enemy Burrobot.nem"
	"artnem/Enemy Buzz Bomber.nem"
	"artnem/Enemy Caterkiller.nem"
	"artnem/Enemy Chopper.nem"
	"artnem/Enemy Crabmeat.nem"
	"artnem/Enemy Jaws.nem"
	"artnem/Enemy Motobug.nem"
	"artnem/Enemy Newtron.nem"
	"artnem/Enemy Orbinaut.nem"
	"artnem/Enemy Roller.nem"
	"artnem/Enemy Splats.nem"
	"artnem/Enemy Yadrin.nem"
	"artnem/Explosion.nem"
	"artnem/Fireballs.nem"
	"artnem/GHZ Breakable Wall.nem"
	"artnem/GHZ Bridge.nem"
	"artnem/GHZ Edge Wall.nem"
	"artnem/GHZ Flower Stalk.nem"
	"artnem/GHZ Giant Ball.nem"
	"artnem/GHZ Purple Rock.nem"
	"artnem/GHZ Spiked Log.nem"
	"artnem/GHZ Swinging Platform.nem"
	"artnem/Game Over.nem"
	"artnem/Giant Ring Flash.nem"
	"artnem/HUD - Life Counter Icon.nem"
	"artnem/HUD.nem"
	"artnem/Hidden Bonuses.nem"
	"artnem/Hidden Japanese Credits.nem"
	"artnem/Invincibility Stars.nem"
	"artnem/LZ 32x16 Block.nem"
	"artnem/LZ 32x32 Block.nem"
	"artnem/LZ Blocks.nem"
	"artnem/LZ Breakable Pole.nem"
	"artnem/LZ Bubbles & Countdown.nem"
	"artnem/LZ Cork.nem"
	"artnem/LZ Flapping Door.nem"
	"artnem/LZ Gargoyle & Fireball.nem"
	"artnem/LZ Harpoon.nem"
	"artnem/LZ Horizontal Door.nem"
	"artnem/LZ Rising Platform.nem"
	"artnem/LZ Spiked Ball & Chain.nem"
	"artnem/LZ Vertical Door.nem"
	"artnem/LZ Water & Splashes.nem"
	"artnem/LZ Water Surface.nem"
	"artnem/LZ Wheel.nem"
	"artnem/Lamppost.nem"
	"artnem/MZ Green Glass Block.nem"
	"artnem/MZ Green Pushable Block.nem"
	"artnem/MZ Lava.nem"
	"artnem/MZ Metal Blocks.nem"
	"artnem/MZ Switch.nem"
	"artnem/Monitors.nem"
	"artnem/Points.nem"
	"artnem/Prison Capsule.nem"
	"artnem/Rings.nem"
	"artnem/SBZ Collapsing Floor.nem"
	"artnem/SBZ Crushing Girder.nem"
	"artnem/SBZ Electrocuter.nem"
	"artnem/SBZ Flaming Pipe.nem"
	"artnem/SBZ Junction Wheel.nem"
	"artnem/SBZ Large Horizontal Door.nem"
	"artnem/SBZ Pizza Cutter.nem"
	"artnem/SBZ Running Disc.nem"
	"artnem/SBZ Sliding Floor Trap.nem"
	"artnem/SBZ Small Vertical Door.nem"
	"artnem/SBZ Spinning Platform.nem"
	"artnem/SBZ Stomper.nem"
	"artnem/SBZ Trapdoor.nem"
	"artnem/SBZ Vanishing Block.nem"
	"artnem/SLZ 32x32 Block.nem"
	"artnem/SLZ Breakable Wall.nem"
	"artnem/SLZ Cannon.nem"
	"artnem/SLZ Fan.nem"
	"artnem/SLZ Little Spikeball.nem"
	"artnem/SLZ Pylon.nem"
	"artnem/SLZ Seesaw.nem"
	"artnem/SLZ Swinging Platform.nem"
	"artnem/SYZ Bumper.nem"
	"artnem/SYZ Large Spikeball.nem"
	"artnem/SYZ Small Spikeball.nem"
	"artnem/Sega Logo (JP1).nem"
	"artnem/Sega Logo.nem"
	"artnem/Shield.nem"
	"artnem/Signpost.nem"
	"artnem/Special 1UP.nem"
	"artnem/Special Birds & Fish.nem"
	"artnem/Special Clouds.nem"
	"artnem/Special Emerald Twinkle.nem"
	"artnem/Special Emeralds.nem"
	"artnem/Special GOAL.nem"
	"artnem/Special Ghost.nem"
	"artnem/Special Glass.nem"
	"artnem/Special R.nem"
	"artnem/Special Red-White.nem"
	"artnem/Special Result Emeralds.nem"
	"artnem/Special UP-DOWN.nem"
	"artnem/Special W.nem"
	"artnem/Special Walls.nem"
	"artnem/Special ZONE1.nem"
	"artnem/Special ZONE2.nem"
	"artnem/Special ZONE3.nem"
	"artnem/Special ZONE4.nem"
	"artnem/Special ZONE5.nem"
	"artnem/Special ZONE6.nem"
	"artnem/Spikes.nem"
	"artnem/Spring Horizontal.nem"
	"artnem/Spring Vertical.nem"
	"artnem/Switch.nem"
	"artnem/Title Cards.nem"
	"artnem/Title Screen Foreground.nem"
	"artnem/Title Screen Sonic.nem"
	"artnem/Title Screen TM.nem"
	"artnem/Unused - Eggman Ending.nem"
	"artnem/Unused - Explosion.nem"
	"artnem/Unused - Fireball.nem"
	"artnem/Unused - GHZ Block.nem"
	"artnem/Unused - GHZ Log.nem"
	"artnem/Unused - Goggles.nem"
	"artnem/Unused - Grass.nem"
	"artnem/Unused - LZ Sonic.nem"
	"artnem/Unused - MZ Background.nem"
	"artnem/Unused - SStage Flash.nem"
	"artnem/Unused - SYZ Sparkles.nem"
	"artnem/Unused - Smoke.nem"

	"artunc/GHZ Flower Large.bin"
	"artunc/GHZ Flower Small.bin"
	"artunc/GHZ Waterfall.bin"
	"artunc/Giant Ring.bin"
	"artunc/HUD Numbers.bin"
	"artunc/Lives Counter Numbers.bin"
	"artunc/MZ Background Torch.bin"
	"artunc/MZ Lava Surface.bin"
	"artunc/MZ Lava.bin"
	"artunc/SBZ Background Smoke.bin"
	"artunc/Sonic.bin"
	"artunc/menutext.bin"

	"collide/Angle Map.bin"
	"collide/Collision Array (Normal).bin"
	"collide/Collision Array (Rotated).bin"
	"collide/GHZ.bin"
	"collide/LZ.bin"
	"collide/MZ.bin"
	"collide/SBZ.bin"
	"collide/SLZ.bin"
	"collide/SYZ.bin"

	"demodata/Ending - GHZ1.bin"
	"demodata/Ending - GHZ2.bin"
	"demodata/Ending - LZ.bin"
	"demodata/Ending - MZ.bin"
	"demodata/Ending - SBZ1.bin"
	"demodata/Ending - SBZ2.bin"
	"demodata/Ending - SLZ.bin"
	"demodata/Ending - SYZ.bin"
	"demodata/Intro - GHZ.bin"
	"demodata/Intro - MZ.bin"
	"demodata/Intro - SYZ.bin"
	"demodata/Intro - Special Stage.bin"

	"levels/ending.bin"
	"levels/ghz1.bin"
	"levels/ghz2.bin"
	"levels/ghz3.bin"
	"levels/ghzbg.bin"
	"levels/lz1.bin"
	"levels/lz2.bin"
	"levels/lz3.bin"
	"levels/lzbg.bin"
	"levels/mz1.bin"
	"levels/mz1bg.bin"
	"levels/mz2.bin"
	"levels/mz2bg.bin"
	"levels/mz3.bin"
	"levels/mz3bg.bin"
	"levels/sbz1.bin"
	"levels/sbz1bg.bin"
	"levels/sbz2.bin"
	"levels/sbz2bg.bin"
	"levels/sbz3.bin"
	"levels/slz1.bin"
	"levels/slz2.bin"
	"levels/slz3.bin"
	"levels/slzbg.bin"
	"levels/syz1.bin"
	"levels/syz2.bin"
	"levels/syz3.bin"
	"levels/syzbg (JP1).bin"
	"levels/syzbg.bin"

	"map16/GHZ.eni"
	"map16/LZ.eni"
	"map16/MZ.eni"
	"map16/SBZ.eni"
	"map16/SLZ.eni"
	"map16/SYZ.eni"

	"map256/GHZ.kos"
	"map256/LZ.kos"
	"map256/MZ (JP1).kos"
	"map256/MZ.kos"
	"map256/SBZ (JP1).kos"
	"map256/SBZ.kos"
	"map256/SLZ.kos"
	"map256/SYZ.kos"

	"misc/Demo Level Order - Ending.bin"
	"misc/Demo Level Order - Intro.bin"
	"misc/GHZ Collapsing Ledge Heightmap.bin"
	"misc/Level Select Text (JP1).bin"
	"misc/Level Select Text.bin"
	"misc/angles.bin"
	"misc/ghzbend1.bin"
	"misc/ghzbend2.bin"
	"misc/mz_pfm1.bin"
	"misc/mz_pfm2.bin"
	"misc/mz_pfm3.bin"
	"misc/sinewave.bin"
	"misc/slzssaw1.bin"
	"misc/slzssaw2.bin"

	"objpos/ending.bin"
	"objpos/fz.bin"
	"objpos/ghz1.bin"
	"objpos/ghz2.bin"
	"objpos/ghz3 (JP1).bin"
	"objpos/ghz3.bin"
	"objpos/lz1 (JP1).bin"
	"objpos/lz1.bin"
	"objpos/lz1pf1.bin"
	"objpos/lz1pf2.bin"
	"objpos/lz2.bin"
	"objpos/lz2pf1.bin"
	"objpos/lz2pf2.bin"
	"objpos/lz3 (JP1).bin"
	"objpos/lz3.bin"
	"objpos/lz3pf1.bin"
	"objpos/lz3pf2.bin"
	"objpos/mz1 (JP1).bin"
	"objpos/mz1.bin"
	"objpos/mz2.bin"
	"objpos/mz3.bin"
	"objpos/sbz1 (JP1).bin"
	"objpos/sbz1.bin"
	"objpos/sbz1pf1.bin"
	"objpos/sbz1pf2.bin"
	"objpos/sbz1pf3.bin"
	"objpos/sbz1pf4.bin"
	"objpos/sbz1pf5.bin"
	"objpos/sbz1pf6.bin"
	"objpos/sbz2.bin"
	"objpos/sbz3.bin"
	"objpos/slz1.bin"
	"objpos/slz2.bin"
	"objpos/slz3.bin"
	"objpos/syz1.bin"
	"objpos/syz2.bin"
	"objpos/syz3 (JP1).bin"
	"objpos/syz3.bin"

	"palette/Cycle - GHZ.bin"
	"palette/Cycle - LZ Conveyor Belt Underwater.bin"
	"palette/Cycle - LZ Conveyor Belt.bin"
	"palette/Cycle - LZ Waterfall.bin"
	"palette/Cycle - MZ (Unused).bin"
	"palette/Cycle - SBZ 1.bin"
	"palette/Cycle - SBZ 10.bin"
	"palette/Cycle - SBZ 2.bin"
	"palette/Cycle - SBZ 3.bin"
	"palette/Cycle - SBZ 4.bin"
	"palette/Cycle - SBZ 5.bin"
	"palette/Cycle - SBZ 6.bin"
	"palette/Cycle - SBZ 7.bin"
	"palette/Cycle - SBZ 8.bin"
	"palette/Cycle - SBZ 9.bin"
	"palette/Cycle - SBZ3 Waterfall.bin"
	"palette/Cycle - SLZ.bin"
	"palette/Cycle - SYZ1.bin"
	"palette/Cycle - SYZ2.bin"
	"palette/Cycle - Special Stage 1.bin"
	"palette/Cycle - Special Stage 2.bin"
	"palette/Cycle - Title Screen Water.bin"
	"palette/Ending.bin"
	"palette/Green Hill Zone.bin"
	"palette/Labyrinth Zone Underwater.bin"
	"palette/Labyrinth Zone.bin"
	"palette/Level Select.bin"
	"palette/Marble Zone.bin"
	"palette/SBZ Act 1.bin"
	"palette/SBZ Act 2.bin"
	"palette/SBZ Act 3 Underwater.bin"
	"palette/SBZ Act 3.bin"
	"palette/Sega Background.bin"
	"palette/Sega1.bin"
	"palette/Sega2.bin"
	"palette/Sonic - LZ Underwater.bin"
	"palette/Sonic - SBZ3 Underwater.bin"
	"palette/Sonic.bin"
	"palette/Special Stage Continue Bonus.bin"
	"palette/Special Stage Results.bin"
	"palette/Special Stage.bin"
	"palette/Spring Yard Zone.bin"
	"palette/Star Light Zone.bin"
	"palette/Title Screen.bin"

	"sound/dac/kick.dpcm"
	"sound/dac/sega.pcm"
	"sound/dac/snare.dpcm"
	"sound/dac/timpani.dpcm"

	"sound/psg/psg1.bin"
	"sound/psg/psg2.bin"
	"sound/psg/psg3.bin"
	"sound/psg/psg4.bin"
	"sound/psg/psg5.bin"
	"sound/psg/psg6.bin"
	"sound/psg/psg7.bin"
	"sound/psg/psg8.bin"
	"sound/psg/psg9.bin"

	"sslayout/1.eni"
	"sslayout/2.eni"
	"sslayout/3.eni"
	"sslayout/4.eni"
	"sslayout/5 (JP1).eni"
	"sslayout/5.eni"
	"sslayout/6 (JP1).eni"
	"sslayout/6.eni"

	"startpos/end1.bin"
	"startpos/end2.bin"
	"startpos/fz.bin"
	"startpos/ghz1 (Credits demo 1).bin"
	"startpos/ghz1 (Credits demo 2).bin"
	"startpos/ghz1.bin"
	"startpos/ghz2.bin"
	"startpos/ghz3.bin"
	"startpos/lz1.bin"
	"startpos/lz2.bin"
	"startpos/lz3 (Credits demo).bin"
	"startpos/lz3.bin"
	"startpos/mz1.bin"
	"startpos/mz2 (Credits demo).bin"
	"startpos/mz2.bin"
	"startpos/mz3.bin"
	"startpos/sbz1 (Credits demo).bin"
	"startpos/sbz1.bin"
	"startpos/sbz2 (Credits demo).bin"
	"startpos/sbz2.bin"
	"startpos/sbz3.bin"
	"startpos/slz1.bin"
	"startpos/slz2.bin"
	"startpos/slz3 (Credits demo).bin"
	"startpos/slz3.bin"
	"startpos/ss1.bin"
	"startpos/ss2.bin"
	"startpos/ss3.bin"
	"startpos/ss4.bin"
	"startpos/ss5.bin"
	"startpos/ss6.bin"
	"startpos/syz1.bin"
	"startpos/syz2.bin"
	"startpos/syz3 (Credits demo).bin"
	"startpos/syz3.bin"
	
	"tilemaps/Hidden Japanese Credits.eni"
	"tilemaps/SS Background 1.eni"
	"tilemaps/SS Background 2.eni"
	"tilemaps/Sega Logo (JP1).eni"
	"tilemaps/Sega Logo.eni"
	"tilemaps/Title Screen.eni"
)

set(ASSET_ANIMS
	"_anim/Ball Hog.asm"
	"_anim/Basaran.asm"
	"_anim/Bomb Enemy.asm"
	"_anim/Bubbles.asm"
	"_anim/Bumper.asm"
	"_anim/Burning Grass.asm"
	"_anim/Burrobot.asm"
	"_anim/Buzz Bomber Missile.asm"
	"_anim/Buzz Bomber.asm"
	"_anim/Caterkiller.asm"
	"_anim/Chopper.asm"
	"_anim/Continue Screen Sonic.asm"
	"_anim/Crabmeat.asm"
	"_anim/Drowning Countdown.asm"
	"_anim/Eggman - Scrap Brain 2 & Final.asm"
	"_anim/Eggman.asm"
	"_anim/Electrocuter.asm"
	"_anim/Ending Sequence Sonic.asm"
	"_anim/FZ Eggman in Ship.asm"
	"_anim/Fireballs.asm"
	"_anim/Flamethrower.asm"
	"_anim/Flapping Door.asm"
	"_anim/Harpoon.asm"
	"_anim/Jaws.asm"
	"_anim/Lava Geyser.asm"
	"_anim/Monitor.asm"
	"_anim/Moto Bug.asm"
	"_anim/Newtron.asm"
	"_anim/Orbinaut.asm"
	"_anim/Plasma Ball Launcher.asm"
	"_anim/Plasma Balls.asm"
	"_anim/Press Start and TM.asm"
	"_anim/Prison Capsule.asm"
	"_anim/Rings.asm"
	"_anim/Roller.asm"
	"_anim/SBZ Small Door.asm"
	"_anim/SBZ Spin Platform Conveyor.asm"
	"_anim/SBZ Spinning Platforms.asm"
	"_anim/SBZ Vanishing Platforms.asm"
	"_anim/Shield and Invincibility.asm"
	"_anim/Signpost.asm"
	"_anim/Sonic (without frame IDs).asm"
	"_anim/Sonic.asm"
	"_anim/Special Stage Entry (Unused).asm"
	"_anim/Springs.asm"
	"_anim/Title Screen Sonic.asm"
	"_anim/Try Again & End Eggman.asm"
	"_anim/Wall of Lava.asm"
	"_anim/Water Splash.asm"
	"_anim/Waterfalls.asm"
	"_anim/Yadrin.asm"
)

set(ASSET_MAPS
	"_maps/Animals 1.asm"
	"_maps/Animals 2.asm"
	"_maps/Animals 3.asm"
	"_maps/Ball Hog.asm"
	"_maps/Basaran.asm"
	"_maps/Big Spiked Ball.asm"
	"_maps/Bomb Enemy.asm"
	"_maps/Boss Items.asm"
	"_maps/Bridge.asm"
	"_maps/Bubbles.asm"
	"_maps/Bumper.asm"
	"_maps/Burrobot.asm"
	"_maps/Button.asm"
	"_maps/Buzz Bomber Missile Dissolve.asm"
	"_maps/Buzz Bomber Missile.asm"
	"_maps/Buzz Bomber.asm"
	"_maps/Caterkiller.asm"
	"_maps/Chained Stompers.asm"
	"_maps/Chopper.asm"
	"_maps/Collapsing Floors.asm"
	"_maps/Collapsing Ledge.asm"
	"_maps/Continue Screen.asm"
	"_maps/Crabmeat.asm"
	"_maps/Credits.asm"
	"_maps/Drowning Countdown.asm"
	"_maps/Eggman - Scrap Brain 2.asm"
	"_maps/Eggman.asm"
	"_maps/Electrocuter.asm"
	"_maps/Ending Sequence Emeralds.asm"
	"_maps/Ending Sequence STH.asm"
	"_maps/Ending Sequence Sonic.asm"
	"_maps/Explosions.asm"
	"_maps/FZ Damaged Eggmobile.asm"
	"_maps/FZ Eggman's Cylinders.asm"
	"_maps/FZ Eggmobile Legs.asm"
	"_maps/Fan.asm"
	"_maps/Fireballs.asm"
	"_maps/Flamethrower.asm"
	"_maps/Flapping Door.asm"
	"_maps/Floating Blocks and Doors.asm"
	"_maps/GHZ Ball.asm"
	"_maps/GHZ Edge Walls.asm"
	"_maps/Game Over.asm"
	"_maps/Gargoyle.asm"
	"_maps/Giant Ring.asm"
	"_maps/Girder Block.asm"
	"_maps/HUD.asm"
	"_maps/Harpoon.asm"
	"_maps/Hidden Bonuses.asm"
	"_maps/Invisible Barriers.asm"
	"_maps/Jaws.asm"
	"_maps/LZ Blocks.asm"
	"_maps/LZ Conveyor.asm"
	"_maps/Lamppost.asm"
	"_maps/Lava Geyser.asm"
	"_maps/Lava Tag.asm"
	"_maps/Light.asm"
	"_maps/MZ Bricks.asm"
	"_maps/MZ Large Grassy Platforms.asm"
	"_maps/MZ Large Green Glass Blocks.asm"
	"_maps/Monitor.asm"
	"_maps/Moto Bug.asm"
	"_maps/Moving Blocks (LZ).asm"
	"_maps/Moving Blocks (MZ and SBZ).asm"
	"_maps/Newtron.asm"
	"_maps/Orbinaut.asm"
	"_maps/Plasma Ball Launcher.asm"
	"_maps/Plasma Balls.asm"
	"_maps/Platforms (GHZ).asm"
	"_maps/Platforms (SLZ).asm"
	"_maps/Platforms (SYZ).asm"
	"_maps/Platforms (unused).asm"
	"_maps/Points.asm"
	"_maps/Pole that Breaks.asm"
	"_maps/Press Start and TM.asm"
	"_maps/Prison Capsule.asm"
	"_maps/Purple Rock.asm"
	"_maps/Pushable Blocks.asm"
	"_maps/Pylon.asm"
	"_maps/Ring Flash.asm"
	"_maps/Rings (JP1).asm"
	"_maps/Rings.asm"
	"_maps/Roller.asm"
	"_maps/Rotating Junction.asm"
	"_maps/Running Disc.asm"
	"_maps/SBZ Eggman's Crumbling Floor.asm"
	"_maps/SBZ Small Door.asm"
	"_maps/SBZ Spinning Platforms.asm"
	"_maps/SBZ Stomper and Door.asm"
	"_maps/SBZ Vanishing Platforms.asm"
	"_maps/SLZ Boss Spikeball.asm"
	"_maps/SLZ Circling Platform.asm"
	"_maps/SLZ Elevators.asm"
	"_maps/SS Chaos Emeralds.asm"
	"_maps/SS DOWN Block.asm"
	"_maps/SS Glass Block.asm"
	"_maps/SS R Block.asm"
	"_maps/SS Result Chaos Emeralds.asm"
	"_maps/SS UP Block.asm"
	"_maps/SS Walls.asm"
	"_maps/SYZ Boss Blocks.asm"
	"_maps/Saws and Pizza Cutters.asm"
	"_maps/Scenery.asm"
	"_maps/Seesaw Ball.asm"
	"_maps/Seesaw.asm"
	"_maps/Shield and Invincibility.asm"
	"_maps/Sideways Stomper.asm"
	"_maps/Signpost.asm"
	"_maps/Smashable Green Block.asm"
	"_maps/Smashable Walls.asm"
	"_maps/Sonic - Dynamic Gfx Script.asm"
	"_maps/Sonic.asm"
	"_maps/Special Stage Entry (Unused).asm"
	"_maps/Spiked Ball and Chain (LZ).asm"
	"_maps/Spiked Ball and Chain (SYZ).asm"
	"_maps/Spiked Pole Helix.asm"
	"_maps/Spikes.asm"
	"_maps/Springs.asm"
	"_maps/Staircase.asm"
	"_maps/Swinging Platforms (GHZ).asm"
	"_maps/Swinging Platforms (SLZ).asm"
	"_maps/Title Screen Sonic.asm"
	"_maps/Trapdoor.asm"
	"_maps/Try Again & End Eggman.asm"
	"_maps/Unused Switch.asm"
	"_maps/Wall of Lava.asm"
	"_maps/Water Splash.asm"
	"_maps/Water Surface.asm"
	"_maps/Waterfalls.asm"
	"_maps/Yadrin.asm"
)

set(ASSETS_MUSIC
	"sound/music/Mus81 - GHZ.asm"
	"sound/music/Mus82 - LZ.asm"
	"sound/music/Mus83 - MZ.asm"
	"sound/music/Mus84 - SLZ.asm"
	"sound/music/Mus85 - SYZ.asm"
	"sound/music/Mus86 - SBZ.asm"
	"sound/music/Mus87 - Invincibility.asm"
	"sound/music/Mus88 - Extra Life.asm"
	"sound/music/Mus89 - Special Stage.asm"
	"sound/music/Mus8A - Title Screen.asm"
	"sound/music/Mus8B - Ending.asm"
	"sound/music/Mus8C - Boss.asm"
	"sound/music/Mus8D - FZ.asm"
	"sound/music/Mus8E - Sonic Got Through.asm"
	"sound/music/Mus8F - Game Over.asm"
	"sound/music/Mus90 - Continue Screen.asm"
	"sound/music/Mus91 - Credits.asm"
	"sound/music/Mus92 - Drowning.asm"
	"sound/music/Mus93 - Get Emerald.asm"
)

set(ASSETS_SFX
	"sound/sfx/SndA0 - Jump.asm"
	"sound/sfx/SndA1 - Lamppost.asm"
	"sound/sfx/SndA2.asm"
	"sound/sfx/SndA3 - Death.asm"
	"sound/sfx/SndA4 - Skid.asm"
	"sound/sfx/SndA5.asm"
	"sound/sfx/SndA6 - Hit Spikes.asm"
	"sound/sfx/SndA7 - Push Block.asm"
	"sound/sfx/SndA8 - SS Goal.asm"
	"sound/sfx/SndA9 - SS Item.asm"
	"sound/sfx/SndAA - Splash.asm"
	"sound/sfx/SndAB.asm"
	"sound/sfx/SndAC - Hit Boss.asm"
	"sound/sfx/SndAD - Get Bubble.asm"
	"sound/sfx/SndAE - Fireball.asm"
	"sound/sfx/SndAF - Shield.asm"
	"sound/sfx/SndB0 - Saw.asm"
	"sound/sfx/SndB1 - Electric.asm"
	"sound/sfx/SndB2 - Drown Death.asm"
	"sound/sfx/SndB3 - Flamethrower.asm"
	"sound/sfx/SndB4 - Bumper.asm"
	"sound/sfx/SndB5 - Ring.asm"
	"sound/sfx/SndB6 - Spikes Move.asm"
	"sound/sfx/SndB7 - Rumbling.asm"
	"sound/sfx/SndB8.asm"
	"sound/sfx/SndB9 - Collapse.asm"
	"sound/sfx/SndBA - SS Glass.asm"
	"sound/sfx/SndBB - Door.asm"
	"sound/sfx/SndBC - Teleport.asm"
	"sound/sfx/SndBD - ChainStomp.asm"
	"sound/sfx/SndBE - Roll.asm"
	"sound/sfx/SndBF - Get Continue.asm"
	"sound/sfx/SndC0 - Basaran Flap.asm"
	"sound/sfx/SndC1 - Break Item.asm"
	"sound/sfx/SndC2 - Drown Warning.asm"
	"sound/sfx/SndC3 - Giant Ring.asm"
	"sound/sfx/SndC4 - Bomb.asm"
	"sound/sfx/SndC5 - Cash Register.asm"
	"sound/sfx/SndC6 - Ring Loss.asm"
	"sound/sfx/SndC7 - Chain Rising.asm"
	"sound/sfx/SndC8 - Burning.asm"
	"sound/sfx/SndC9 - Hidden Bonus.asm"
	"sound/sfx/SndCA - Enter SS.asm"
	"sound/sfx/SndCB - Wall Smash.asm"
	"sound/sfx/SndCC - Spring.asm"
	"sound/sfx/SndCD - Switch.asm"
	"sound/sfx/SndCE - Ring Left Speaker.asm"
	"sound/sfx/SndCF - Signpost.asm"
	"sound/sfx/SndD0 - Waterfall.asm"
)

set(ASSETS_OUT "")
set(ASSETS_ASM "")

# Convert the binary assets to C arrays
foreach (ASSET_BIN ${ASSET_BINS})
	set(ASSET_IN "${CMAKE_CURRENT_SOURCE_DIR}/Disasm/${ASSET_BIN}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Assets/${ASSET_BIN}.h")

	# Make sure the output directory exists
	get_filename_component(ASSET_OUT_DIR "${ASSET_OUT}" DIRECTORY)
	file(MAKE_DIRECTORY "${ASSET_OUT_DIR}")

	add_custom_command(
		OUTPUT "${ASSET_OUT}"
		COMMAND bin2h "${ASSET_IN}" "${ASSET_OUT}"
		DEPENDS "${ASSET_IN}"
		COMMENT "Converting ${ASSET_BIN} to C array"
	)

	list(APPEND ASSETS_OUT "${ASSET_OUT}")
endforeach()

# Wrap each different assembly asset in the correct wrapper
set(DISASM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Disasm")

foreach (ASSET_ANIM ${ASSET_ANIMS})
	set(ASSET_IN "${CMAKE_CURRENT_SOURCE_DIR}/Disasm/${ASSET_ANIM}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Asm/${ASSET_ANIM}")

	set(ASM_TO_BUILD "${ASSET_ANIM}")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Asm/General.asm.in" "${ASSET_OUT}")

	list(APPEND ASSETS_ASM "${ASSET_ANIM}")
endforeach()

foreach (ASSET_MAP ${ASSET_MAPS})
	set(ASSET_IN "${CMAKE_CURRENT_SOURCE_DIR}/Disasm/${ASSET_MAP}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Asm/${ASSET_MAP}")

	set(ASM_TO_BUILD "${ASSET_MAP}")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Asm/General.asm.in" "${ASSET_OUT}")

	list(APPEND ASSETS_ASM "${ASSET_MAP}")
endforeach()

foreach (ASSET_MUSIC ${ASSETS_MUSIC})
	set(ASSET_IN "${CMAKE_CURRENT_SOURCE_DIR}/Disasm/${ASSET_MUSIC}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Asm/${ASSET_MUSIC}")

	set(ASM_TO_BUILD "${ASSET_MUSIC}")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Asm/SMPS.asm.in" "${ASSET_OUT}")

	list(APPEND ASSETS_ASM "${ASSET_MUSIC}")
endforeach()

foreach (ASSET_SFX ${ASSETS_SFX})
	set(ASSET_IN "${CMAKE_CURRENT_SOURCE_DIR}/Disasm/${ASSET_SFX}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Asm/${ASSET_SFX}")

	set(ASM_TO_BUILD "${ASSET_SFX}")
	configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Asm/SMPS.asm.in" "${ASSET_OUT}")

	list(APPEND ASSETS_ASM "${ASSET_SFX}")
endforeach()

# Assemble the assets
foreach (ASSET_ASM ${ASSETS_ASM})
	set(ASSET_IN "${CMAKE_CURRENT_BINARY_DIR}/Asm/${ASSET_ASM}")
	set(ASSET_OUT "${CMAKE_CURRENT_BINARY_DIR}/Assets/${ASSET_ASM}.h")

	# Make sure the output directory exists
	get_filename_component(ASSET_OUT_DIR "${ASSET_OUT}" DIRECTORY)
	file(MAKE_DIRECTORY "${ASSET_OUT_DIR}")

	add_custom_command(
		OUTPUT "${ASSET_IN}.p"
		COMMAND ${AS_PATH} -xx -n -q -A -L -U -E -i . -o "${ASSET_IN}.p" "${ASSET_IN}"
		DEPENDS "${ASSET_IN}"
		WORKING_DIRECTORY "${DISASM_PATH}"
		COMMENT "Assembling ${ASSET_ASM}"
	)

	add_custom_command(
		OUTPUT "${ASSET_IN}.bin"
		COMMAND ${P2BIN_PATH} "${ASSET_IN}.p" "${ASSET_IN}.bin"
		DEPENDS "${ASSET_IN}.p"
		COMMENT "Converting ${ASSET_ASM} to binary"
	)

	add_custom_command(
		OUTPUT "${ASSET_OUT}"
		COMMAND bin2h "${ASSET_IN}.bin" "${ASSET_OUT}"
		DEPENDS "${ASSET_IN}.bin"
		COMMENT "Converting ${ASSET_ASM} to C array"
	)

	list(APPEND ASSETS_OUT "${ASSET_OUT}")
endforeach()

# Depend on the assets
add_custom_target(Sonic1C.Assets DEPENDS ${ASSETS_OUT})
add_dependencies(Sonic1C.Game Sonic1C.Assets)
