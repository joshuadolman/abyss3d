#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 720

#define VIDEO_BUFFER_BYTES_PER_PIXEL 4

#define SINE_FUNC sinf 

#include <sys/mman.h>

#include "libs/types.h"
#include "abyss/abyss_math.h"

struct ___bitmap {
	vec4_u8* pixels;
	vec2_u16 dim;
};


global ___bitmap globalMyBitmap = {};
global ___bitmap amazonBitmap = {};

#include "sdl/sdl_os.cpp"

#include "abyss/abyss.cpp"
#include "abyss/abyss_entity.cpp"

global b32 globalRunning;
global entity globalMyEntity;

internal void
SDL_CleanUp(SDL_Window* window, SDL_Renderer* renderer) {
	if (window) { SDL_DestroyRenderer(renderer); }
	if (renderer) { SDL_DestroyWindow(window); }
	
	SDL_Quit();
}

inline void OutputDebug(const char* s) {
#if DEBUG_OUTPUT
#if OS_LINUX
	printf(s);
#endif
#endif
	return;
}

int
main(void) {
	SDL_Window* window = 0;
	SDL_Renderer* renderer = 0;
	
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		OutputDebug("hellowwolr \n");
		return 0;
	} else {
		OutputDebug("aoaoaoa");
		window = SDL_CreateWindow("Abyss Engine",
										  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
										  WINDOW_WIDTH, WINDOW_HEIGHT,
										  SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/);
		if (window == NULL )
		{
			//TODO: ERROR
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL )
			{
				//TODO: ERROR
			} else {
				if (Mix_OpenAudio( 48000, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
				{
					return -1;
				}
				
				IMG_Init(IMG_INIT_PNG);
				
				SDL_Surface* surface = IMG_Load("d2_isotile_test_256x128.png");
				globalMyBitmap.pixels = (vec4_u8*)surface->pixels;
				globalMyBitmap.dim.w = surface->w;
				globalMyBitmap.dim.h = surface->h;
				
				SDL_Surface* AmazonSurface = IMG_Load("d2_amazon_test_idle.png");
				if (AmazonSurface) {
					printf("it work!\n");
				} else {
					printf("oh no\n");
				}
				amazonBitmap.pixels = (vec4_u8*)AmazonSurface->pixels;
				amazonBitmap.dim.w = AmazonSurface->w;
				amazonBitmap.dim.h = AmazonSurface->h;
				
				game_buffer_video_output gameVideoBufferOutput = { 0 };
				gameVideoBufferOutput.width = WINDOW_WIDTH;
				gameVideoBufferOutput.height = WINDOW_HEIGHT;
				gameVideoBufferOutput.memorySize = WINDOW_HEIGHT * WINDOW_WIDTH * VIDEO_BUFFER_BYTES_PER_PIXEL;
				// gameVideoBufferOutput.memory = mmap(0, gameVideoBufferOutput.memorySize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            
				SDL_Surface* sdl_video_buffer = SDL_CreateRGBSurfaceWithFormat(0,
																									WINDOW_WIDTH, WINDOW_HEIGHT, VIDEO_BUFFER_BYTES_PER_PIXEL*8,
																									SDL_PIXELFORMAT_ABGR8888);
				gameVideoBufferOutput.memory = sdl_video_buffer->pixels;
				
				game_memory gameMemory = {0};
				gameMemory.permanentStorageSize = Megabytes(128);
				gameMemory.transientStorageSize = Gigabytes(1);
				u64 totalSize = gameMemory.permanentStorageSize + gameMemory.transientStorageSize;
				gameMemory.permanentStorage = OS_AllocateMemory(0, totalSize);
				gameMemory.transientStorage = ((u8*)gameMemory.permanentStorageSize + gameMemory.permanentStorageSize);
				gameMemory.isInitialized = true;
				
				game_input gameInput = {0};
				game_buffer_sound_output gameSoundBufferOutput = {0};
				//gameSoundBufferOutput.sampleCountToOutput = 
				gameSoundBufferOutput.samplesPerSecond = 48000;
				gameSoundBufferOutput.samples = (s16*)OS_AllocateMemory(0,
																						  gameSoundBufferOutput.samplesPerSecond * sizeof(s16) * 2);
				
				//SDL_BlitSurface(sdl_video_buffer, NULL, SDL_GetWindowSurface(window), &dstRect);
				SDL_Texture* blittingTexture = SDL_CreateTextureFromSurface(renderer, sdl_video_buffer);
				SDL_Event event;
				globalRunning = true;
				while(globalRunning)
				{
					while(SDL_PollEvent(&event))
					{
						switch (event.type)
						{
							case SDL_KEYDOWN:
							{
								
							} break;
							case SDL_QUIT:
							{
								globalRunning = false;
							} break;
							default:
							{
								
							} break;
						}
					}
					
					
					
					GameUpdateAndRender(&gameMemory, &gameInput, &gameVideoBufferOutput, &gameSoundBufferOutput);
					
					if (blittingTexture!=NULL) {
						SDL_DestroyTexture(blittingTexture);
						blittingTexture = {0};
					}
					blittingTexture = SDL_CreateTextureFromSurface(renderer, sdl_video_buffer);
					SDL_RenderCopy(renderer, blittingTexture, NULL, NULL);
					SDL_RenderPresent(renderer);
				}
				if (blittingTexture) { SDL_DestroyTexture(blittingTexture); }
			}
		}
	}
	SDL_CleanUp(window, renderer);
}