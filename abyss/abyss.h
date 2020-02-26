#ifndef ABYSS_H
#define ABYSS_H


//Services the platform provides to the game
struct TEMP_game_file {
	u32 dataSize;
	void* data;
};

internal TEMP_game_file TEMP_PlatformReadEntireFile(char* filename);
internal void TEMP_PlatformFreeFileMemory(void* fileData);

internal b32 TEMP_PlatformWriteEntireFile(char* filename, TEMP_game_file* file);
//Services the game provides to the platform
struct game_buffer_sound_output
{
	s32 sampleCountToOutput;
	s32 samplesPerSecond;
	s16* samples;
};

struct game_buffer_video_output
{
	s32 width;
	s32 height;
	u32 memorySize;
	void* memory;
};

struct game_button_state
{
	s16 halfTransitions;
	b16 isDown;
};

struct game_stick_axis_state
{
	f32 start;
	f32 end;
	f32 min;
	f32 max;
};

struct game_controller_input
{
	game_stick_axis_state stickLeftX;
	game_stick_axis_state stickLeftY;
	game_stick_axis_state stickRightX;
	game_stick_axis_state stickRightY;
	
	f32 triggerRight;
	f32 triggerLeft;
	
	union {
		game_button_state buttons[12];
		struct {
			game_button_state buttonA;
			game_button_state buttonB;
			game_button_state buttonX;
			game_button_state buttonY;
			
			game_button_state dPadUp;
			game_button_state dPadDown;
			game_button_state dPadLeft;
			game_button_state dPadRight;
			
			game_button_state start;
			game_button_state select;
			
			game_button_state shoulderRight;
			game_button_state shoulderLeft;
			
			game_button_state thumbRight;
			game_button_state thumbLeft;
		};
	};
};

struct game_input
{
	game_controller_input controllers[4];
};

struct game_memory
{
	b32 isInitialized;
	
	u64 permanentStorageSize;
	void* permanentStorage;
	u64 transientStorageSize;
	void* transientStorage;
};

struct game_state
{
	
};


internal void GameUpdateAndRender(game_memory* memory, game_input* input, game_buffer_video_output* bufferVideoOuput, game_buffer_sound_output* bufferSoundOutput);

//functions the platform doesnt care about
internal void RenderWeirdGradient(game_buffer_video_output* bufferVideoOuput, s32 xOff, s32 yOff);
internal void GameOutputSound(game_buffer_sound_output* bufferSoundOutput, s32 toneHz);

#endif //ABYSS_H