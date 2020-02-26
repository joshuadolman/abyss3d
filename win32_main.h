#ifndef WIN32_ABYSS_H
#define WIN32_ABYSS_H

#define WINDOW_STYLE WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX|WS_SYSMENU|WS_VISIBLE
#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 360

struct win32_buffer_video_output
{
	BITMAPINFO info;
	s32 width;
	s32 height;
	u32 memorySize;
	void* memory;
    game_buffer_video_output* intermediateBuffer; //This is here so I can reorder the bits from the game rendering to what windows wants (RGBA -> ABGR)
};

struct win32_buffer_sound_output
{
	u32 samplesPerSecond;
	s32 bytesPerSample;
	s32 secondaryBufferSize; //samplesPerSecond*secondaryBufferSize
	u32 latencySampleCount;
	u32 runningSampleIndex;
};

struct win32_rect_dim
{
	s32 x;
	s32 y;
	s32 x2;
	s32 y2;
	s32 width;
	s32 height;
};

global b32 globalRunning;
global s64 globalPerfCountFrequency;

global LPDIRECTSOUNDBUFFER globalSecondarySoundBuffer;

global b32 gotDINPUT;

#endif //WIN32_ABYSS_H