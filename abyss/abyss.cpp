#ifndef ABYSS_CPP
#define ABYSS_CPP

#include "abyss.h"
#include "abyss_math.h"

internal void
RenderWeirdGradient(game_buffer_video_output* buffer, s32 xOff, s32 yOff, b16 down)
{
	u32 bytesPerPixel = 4;
	u32 pitch = buffer->width * VIDEO_BUFFER_BYTES_PER_PIXEL;
	u8* row = (u8*)buffer->memory;
	for(s32 y = 0; y < buffer->height; ++y)
	{
		u8* pixel = (u8*)row;
		for(s32 x = 0; x < buffer->width; ++x)
		{
			if (down) {
				*pixel = 127;
				++pixel;
				
				*pixel = 127;
				++pixel;
				
				*pixel = 127;
				++pixel;
				
				*pixel = 0;
				++pixel;
			} else {
				*pixel = 127;
				++pixel;
				
				*pixel = (u8)(x+xOff);
				++pixel;
				
				*pixel = (u8)(y+yOff);
				++pixel;
				
				*pixel = 0;
				++pixel;
			}
			
		}
		row += pitch;
	}

}

internal void
GameOutputSound(game_buffer_sound_output* bufferSoundOutput, s32 toneHz)
{
	local_persist f32 tSine;
	
	s16 toneVolume = 200;
	//s32 toneHz = 256;
	s32 wavePeriod = 1;
	if(toneHz!=0)
	{
		wavePeriod = bufferSoundOutput->samplesPerSecond/toneHz;
	}
	
	s16* sampleOut = bufferSoundOutput->samples;
	for(s32 sampleIndex = 0; sampleIndex < bufferSoundOutput->sampleCountToOutput; ++sampleIndex)
	{
		f32 sineValue = SINE_FUNC(tSine);
		s16 sampleValue = (s16)(sineValue * toneVolume);
		*sampleOut++ = sampleValue;
		*sampleOut++ = sampleValue;
		
		tSine += TAU32/(f32)wavePeriod;
	}
}

internal void
win32_D3D11_RenderFrame(void);

internal void
GameUpdateAndRender(game_memory* memory, game_input* input,
						  game_buffer_video_output* bufferVideoOutput, game_buffer_sound_output* bufferSoundOutput)
{
	Assert(sizeof(game_state) <= memory->permanentStorageSize);
	
	local_persist s32 xOffset = 1;
	local_persist s32 yOffset = 0;
	s32 toneHz = 256+(s32)(128.0f*input->controllers[0].stickLeftY.end);
	
	Assert(input->controllers[0].buttonA.isDown==false);
	
	//GameOutputSound(bufferSoundOutput, toneHz);
	
	//RenderWeirdGradient(bufferVideoOutput, xOffset, yOffset, input->controllers[0].dPadUp.isDown);
    win32_D3D11_RenderFrame();
}

#endif //ABYSS_CPP