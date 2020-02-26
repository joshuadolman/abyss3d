#include <windows.h>
#include "libs/types.h"



#include <xinput.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <dsound.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSET(x)
//#define STBI_MALLOC
//#define STBI_REALLOC
//#define STBI_FREE
#include "libs/stb_image.h"


#include "abyss/abyss_math.cpp"
#include "abyss/abyss_entity.cpp"

#if CRT==0
#include "win32/win32_crt.c"
#include "libs/sse_math.h"
#define SINE_FUNC mySine

inline f32
mySine(f32 r) {
	//Bhaskara I's sine approximation formula
	//TODO(joshua): Is this actually accurate enough?
	f32 a = 16.0f*r*(PI32-r);
	f32 b = (5.0f*(PI32*PI32)) - 4.0f*r*(PI32-r);
	return a/b;
}
inline f32
mySineAngle(f32 theta) {
	//Bhaskara I's sine approximation formula
	//TODO(joshua): Is this actually accurate enough?
	f32 a = 4.0f*theta*(180.0f-theta);
	f32 b = 405000.0f - theta*(180.0f-theta);
	return a/b;
}
#else
#include <math.h>
#define SINE_FUNC sinf
#include <stdio.h>
#endif

#if DEBUG_OUTPUT == ENABLED
#define win32_OutputDebug OutputDebugString
#else
inline void
win32_OutputDebug(LPCSTR lpOutputString)
{
	return;
}
#endif

#define VIDEO_BUFFER_BYTES_PER_PIXEL 4

#include "abyss/abyss.cpp"
#include "win32_main.h"
#include "win32/win32_d3d11.cpp"

#include "win32/win32_xinput.cpp"


#if RELEASE_MODE==RELEASEMODE_INTERNAL
internal TEMP_game_file
TEMP_PlatformReadEntireFile(char* filename) {
	TEMP_game_file result = {};
	HANDLE fileHandle = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		LARGE_INTEGER fileSize;
		if(GetFileSizeEx(fileHandle, &fileSize))
		{
			
			result.dataSize = SafeTruncateU64ToU32(fileSize.QuadPart);
			result.data = VirtualAlloc(0, result.dataSize, MEM_COMMIT|MEM_COMMIT, PAGE_READWRITE);
			if(result.data)
			{
				DWORD bytesRead = 0;
				if(ReadFile(fileHandle, result.data, result.dataSize, &bytesRead, 0) && (result.dataSize == bytesRead))
				{
					win32_OutputDebug("FILE READ SUCCESS!");
					
				} else {
					win32_OutputDebug("FILE READ FAILED!");
				}
			} else {
				win32_OutputDebug("ALLOC FAILED!");
			}
		} else {
			win32_OutputDebug("GETFILESIZE FAILED!");
		}
		CloseHandle(fileHandle);
	} else {
		win32_OutputDebug("COULD NOT FIND FILE!");
	}
	
	return result;
}

internal void
TEMP_PlatformFreeFileMemory(void* fileData) {
	if(fileData)
	{
		VirtualFree(fileData, 0, MEM_RELEASE);
	} else {
		//TODO(joshua): logging
	}
}

internal b32
TEMP_PlatformWriteEntireFile(char* filename, TEMP_game_file* file) {
	b32 result = false;
	HANDLE fileHandle = CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD bytesWritten;
		if(WriteFile(fileHandle, file->data, file->dataSize, &bytesWritten, 0))
		{
			result = (bytesWritten == file->dataSize);
		} else {
			
		}
		CloseHandle(fileHandle);
	} else {
		
	}
	return result;
}
#endif

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuideDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

internal void
win32_InitDirectSound(HWND window, s32 samplesPerSecond, s32 bufferSize)
{
	HMODULE DirectSoundLib = LoadLibrary("dsound.dll");
	
	if(DirectSoundLib)
	{
		win32_OutputDebug("SUCCESS: Loaded dsound.dll\n");
		direct_sound_create* DirectSoundCreate = (direct_sound_create*)GetProcAddress(DirectSoundLib, "DirectSoundCreate");
		LPDIRECTSOUND DirectSound;
		if(DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
		{
			win32_OutputDebug("SUCCESS: Got procedure address for DirectSoundCreate\n");
			win32_OutputDebug("SUCCESS: Created DirectSound\n");
			
			if( SUCCEEDED(DirectSound->SetCooperativeLevel(window, DSSCL_PRIORITY)))
			{
				win32_OutputDebug("SUCCESS: Set DirectSound CooperativeLevel to DSSCL_PRIORITY\n");
				WAVEFORMATEX waveFormat = {};
				waveFormat.wFormatTag = WAVE_FORMAT_PCM;
				waveFormat.nChannels = 2;
				waveFormat.nSamplesPerSec = samplesPerSecond;
				
				waveFormat.wBitsPerSample = 16;
				waveFormat.nBlockAlign = (waveFormat.nChannels*waveFormat.wBitsPerSample) / 8;
				waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec*waveFormat.nBlockAlign;
				//waveFormat.cbSize = 0;
				
				DSBUFFERDESC bufferDescription = {};
				bufferDescription.dwSize = sizeof(bufferDescription);
				bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
				
				LPDIRECTSOUNDBUFFER primarySoundBuffer;
				if(SUCCEEDED(DirectSound->CreateSoundBuffer(&bufferDescription, &primarySoundBuffer, 0)))
				{
					win32_OutputDebug("SUCCESS: Created primarySoundBuffer\n");
					if(SUCCEEDED(primarySoundBuffer->SetFormat(&waveFormat)))
					{
						win32_OutputDebug("SUCCESS: Set primarySoundBuffer waveFormat\n");
					} else {
						win32_OutputDebug("FAIL: Could not set primarySoundBuffer waveFormat\n");
					}
				} else {
					win32_OutputDebug("FAIL: Could not create primarySoundBuffer\n");
				}
				
				bufferDescription = {};
				bufferDescription.dwSize = sizeof(bufferDescription);
				//bufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;
				bufferDescription.dwBufferBytes = bufferSize;
				bufferDescription.lpwfxFormat = &waveFormat;
				
				if(SUCCEEDED(DirectSound->CreateSoundBuffer(&bufferDescription, &globalSecondarySoundBuffer, 0)))
				{
					win32_OutputDebug("SUCCESS: Created globalSecondarySoundBuffer\n");
				} else {
					win32_OutputDebug("FAIL: Could not create globalSecondarySoundBuffer\n");
				}
			} else {
				win32_OutputDebug("FAIL: Could not set DirectSound CooperativeLevel to DSSCL_PRIORITY\n");
			}
		} else {
			win32_OutputDebug("FAIL: Did not GetProcAddress of DirectSoundCreate or - \n");
			win32_OutputDebug("FAIL: Could not DirectSoundCreate()\n");
		}
	} else {
		win32_OutputDebug("FAIL: Could not load dsound.dll\n");
	}
}

internal win32_rect_dim
win32_GetClientDimensions(HWND window)
{
	win32_rect_dim result;
	RECT clientRect;
	GetClientRect(window, &clientRect);
	
	result.x = clientRect.left;
	result.y = clientRect.top;
	result.x2 = clientRect.right;
	result.y2 = clientRect.bottom;
	result.width = clientRect.right - clientRect.left;
	result.height = clientRect.bottom - clientRect.top;
	
	return result;
}

internal void
win32_ResizeVideoBuffers(win32_buffer_video_output* bufferVideoOutput, game_buffer_video_output* gameVideoBuffer, s32 width, s32 height)
{
	
	if(bufferVideoOutput->memory)
	{
		VirtualFree(bufferVideoOutput->memory, bufferVideoOutput->memorySize, MEM_RELEASE);
	}
    if(bufferVideoOutput->intermediateBuffer->memory)
    {
        VirtualFree(bufferVideoOutput->intermediateBuffer->memory, bufferVideoOutput->memorySize, MEM_RELEASE);
    }

    bufferVideoOutput->width = width;
	bufferVideoOutput->height = height;
	
	bufferVideoOutput->info.bmiHeader.biSize = sizeof(bufferVideoOutput->info.bmiHeader);
	bufferVideoOutput->info.bmiHeader.biWidth = bufferVideoOutput->width;
	bufferVideoOutput->info.bmiHeader.biHeight = -bufferVideoOutput->height;
	bufferVideoOutput->info.bmiHeader.biPlanes = 1;
	bufferVideoOutput->info.bmiHeader.biBitCount = 32;
	bufferVideoOutput->info.bmiHeader.biCompression = BI_RGB;
	//bitmapInfo.bmiHeader.biSizeImage = 0;
	//bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	//bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	//bitmapInfo.bmiHeader.biClrUsed = 0;
	//bitmapInfo.bmiHeader.biClrImportant = 0;
	
	bufferVideoOutput->memorySize = VIDEO_BUFFER_BYTES_PER_PIXEL*(bufferVideoOutput->width*bufferVideoOutput->height);
	bufferVideoOutput->memory = VirtualAlloc(0, bufferVideoOutput->memorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

    bufferVideoOutput->intermediateBuffer->width = width;
    bufferVideoOutput->intermediateBuffer->height = height;
    bufferVideoOutput->intermediateBuffer->memorySize = VIDEO_BUFFER_BYTES_PER_PIXEL*bufferVideoOutput->intermediateBuffer->width*bufferVideoOutput->intermediateBuffer->height;
    bufferVideoOutput->intermediateBuffer->memory = VirtualAlloc(0, bufferVideoOutput->intermediateBuffer->memorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
	
	gameVideoBuffer->width = bufferVideoOutput->width;
	gameVideoBuffer->height = bufferVideoOutput->height;
	gameVideoBuffer->memorySize = bufferVideoOutput->memorySize;
	gameVideoBuffer->memory = bufferVideoOutput->memory;
}

internal void
win32_BlitBackbufferToClient(HDC deviceContext, win32_buffer_video_output* bufferVideoOutput, s32 clientWidth, s32 clientHeight)
{

    int xPixelOffset;
    int yPixelOffset;
    int currentU8Offset;
	for (int x = 0; x < bufferVideoOutput->width; x++)
    {
        for (int y = 0; y < bufferVideoOutput->height; y++)
        {
            for (int channel = 0; channel < 4; channel++)
            {
                xPixelOffset = x*VIDEO_BUFFER_BYTES_PER_PIXEL;
                yPixelOffset = y*VIDEO_BUFFER_BYTES_PER_PIXEL*bufferVideoOutput->width;
                currentU8Offset = xPixelOffset + yPixelOffset + channel;
                *(((u8*)bufferVideoOutput->intermediateBuffer->memory) + currentU8Offset) = *(((u8*)bufferVideoOutput->intermediateBuffer->memory) + currentU8Offset);
            }
        }
    }
    memset(bufferVideoOutput->intermediateBuffer->memory, 0xFF0022FF, bufferVideoOutput->intermediateBuffer->memorySize);
    
    StretchDIBits(deviceContext,
					  0, 0, clientWidth, clientHeight,
					  0, 0, bufferVideoOutput->intermediateBuffer->width, bufferVideoOutput->intermediateBuffer->height,
					  bufferVideoOutput->intermediateBuffer->memory,
					  &bufferVideoOutput->info,
					  DIB_RGB_COLORS, SRCCOPY);
}

internal LRESULT CALLBACK
win32_MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	
	switch(message)
	{
		case WM_DESTROY:
		case WM_CLOSE:
		case WM_QUIT:
		{
			globalRunning = false;
		} break;
		
		case WM_ACTIVATEAPP:
		{} break;
		
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			Assert(false);
		} break;
		
		default:
		{
			result = DefWindowProc(window, message, wParam, lParam);
		} break;
	}
	
	return (result);
}

internal void
win32_ClearSoundBuffer(win32_buffer_sound_output* soundOutput)
{
	void* region1;
	DWORD region1Size;
	void* region2;
	DWORD region2Size;
	if(SUCCEEDED(globalSecondarySoundBuffer->Lock(0, soundOutput->secondaryBufferSize,
																 &region1, &region1Size,
																 &region2, &region2Size, 0)))
	{
		u8* dstSample = (u8*)region1;
		for(DWORD byteIndex = 0; byteIndex < region1Size; ++byteIndex)
		{
			*dstSample++ = 0;
		}
		
		dstSample = (u8*)region2;
		for(DWORD byteIndex = 0; byteIndex < region2Size; ++byteIndex)
		{
			*dstSample++ = 0;
		}
		
		globalSecondarySoundBuffer->Unlock(region1, region1Size, region2, region2Size);
	}
}

internal void
win32_FillSoundBuffer(win32_buffer_sound_output* soundOutput, DWORD byteToLock, DWORD bytesToWrite, game_buffer_sound_output* sourceSoundBuffer)
{
	void* region1;
	DWORD region1Size;
	void* region2;
	DWORD region2Size;
	if(SUCCEEDED(globalSecondarySoundBuffer->Lock(byteToLock, bytesToWrite,
																 &region1, &region1Size,
																 &region2, &region2Size, 0)))
	{
		
		DWORD region1SampleCount = region1Size/soundOutput->bytesPerSample;
		s16* dstSample = (s16*)region1;
		s16* srcSample = sourceSoundBuffer->samples;
		for(DWORD sampleIndex = 0; sampleIndex < region1SampleCount; ++sampleIndex)
		{
			*dstSample++ = *srcSample++;
			*dstSample++ = *srcSample++;
			
			++soundOutput->runningSampleIndex;
		}
		DWORD region2SampleCount = region2Size/soundOutput->bytesPerSample;
		dstSample = (s16*)region2;
		for(DWORD sampleIndex = 0; sampleIndex < region2SampleCount; ++sampleIndex)
		{
			*dstSample++ = *srcSample++;
			*dstSample++ = *srcSample++;
			
			++soundOutput->runningSampleIndex;
		}
		
		globalSecondarySoundBuffer->Unlock(region1, region1Size, region2, region2Size);
	}
}

internal BOOL CALLBACK
win32_GiveFirstController(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	if(!*(LPCDIDEVICEINSTANCE*)pvRef)
	{
		win32_OutputDebug("controller set\n");
		*(LPCDIDEVICEINSTANCE*)pvRef = lpddi;
		win32_OutputDebug("controller set\n");
		gotDINPUT = true;
	} else {
		win32_OutputDebug("controller not set\n");
	}
	
	return DIENUM_CONTINUE;
}

internal void
win32_OutputAudio(win32_buffer_sound_output* win32SoundOutput, game_buffer_sound_output* bufferSoundOutput)
{
	DWORD byteToLock = 0;
	DWORD targetCursor;
	DWORD bytesToWrite = 0;
	DWORD playCursor;
	DWORD writeCursor;
	b32 soundIsValid = false;
	if(SUCCEEDED(globalSecondarySoundBuffer->GetCurrentPosition(&playCursor, &writeCursor)))
	{
		byteToLock = (win32SoundOutput->runningSampleIndex*win32SoundOutput->bytesPerSample) % win32SoundOutput->secondaryBufferSize;
		targetCursor = (playCursor + (win32SoundOutput->latencySampleCount*win32SoundOutput->bytesPerSample)) % win32SoundOutput->secondaryBufferSize;
		bytesToWrite;
		if(byteToLock > targetCursor)
		{
			bytesToWrite = (win32SoundOutput->secondaryBufferSize - byteToLock);
			bytesToWrite += targetCursor;
		} else {
			bytesToWrite = targetCursor - byteToLock;
		}
		
		soundIsValid = true;
	}
	if(soundIsValid)
	{
		bufferSoundOutput->sampleCountToOutput = bytesToWrite / win32SoundOutput->bytesPerSample;
		win32_FillSoundBuffer(win32SoundOutput, byteToLock, bytesToWrite, bufferSoundOutput);
	}
}

internal void
win32_ProcessPendingMessages(HWND window, game_input* newInputState, game_input* oldInputState, game_buffer_video_output* gameVideoBuffer, win32_buffer_video_output* osVideoBuffer)
{
	MSG message;
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		switch(message.message)
		{
			case WM_SIZE:
			{
				win32_OutputDebug("WM_SIZE\n");
				
				win32_rect_dim clientDim = win32_GetClientDimensions(window);
				//win32_ResizeDIBSection(&win32BufferVideoOutput, clientDim.width, clientDim.height);
				win32_ResizeVideoBuffers(osVideoBuffer, gameVideoBuffer, clientDim.width, clientDim.height);
				//RenderWeirdGradient(&win32BufferVideoOutput, xOffset, yOffset);
				
				HDC deviceContext = GetDC(window);
				win32_BlitBackbufferToClient(deviceContext, osVideoBuffer, clientDim.width, clientDim.height);
				ReleaseDC(window, deviceContext);
			} break;
			
			case WM_PAINT:
			{
				win32_OutputDebug("WM_PAINT\n");
				
				PAINTSTRUCT paint;
				HDC deviceContext = BeginPaint(window, &paint);
				
				win32_rect_dim clientDim = win32_GetClientDimensions(window);
				win32_BlitBackbufferToClient(deviceContext, osVideoBuffer, clientDim.width, clientDim.height);
				
				EndPaint(window, &paint);
			} break;
			
			case WM_QUIT:
			{
				globalRunning = false;
			} break;
			
			case WM_SYSKEYUP:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				u64 VKCode = message.wParam;
				b32 wasDown = ((message.lParam & (1 << 30)) != 0);
				b32 isDown = ((message.lParam & (1 << 31)) == 0);
				char strbuffer[255];
                sprintf_s(strbuffer, sizeof(strbuffer), "%c\n", (int)VKCode);
                win32_OutputDebug(strbuffer);
                if(wasDown != isDown)
				{
					if(VKCode == 'W')
					{
						win32_OutputDebug("W: ");
                        if(isDown)
						{
                            TEMP_win32_D3D11_UpdateCamera(-1.0f);
							win32_OutputDebug("pressed");
							newInputState->controllers[0].dPadUp.isDown=TRUE;
						}
						if(wasDown)
						{
                            TEMP_win32_D3D11_UpdateCamera(-10.0f);
							win32_OutputDebug("released");
							newInputState->controllers[0].dPadUp.isDown=FALSE;
						}
						win32_OutputDebug("\n");
					}
					else if(VKCode == 'A')
					{
						//keyboardController->dPadLeft.isDown = !keyboardController->dPadLeft.isDown;
                        
					}
					else if(VKCode == 'S')
					{
                        win32_OutputDebug("S: ");
                        TEMP_win32_D3D11_UpdateCamera(-5.0f);
                        if(isDown)
						{
                            
							win32_OutputDebug("pressed");
							
						}
						if(wasDown)
						{
                            // TEMP_win32_D3D11_UpdateCamera(-10.0f);
							win32_OutputDebug("released");
							
						}
						//keyboardController->dPadDown.isDown = !keyboardController->dPadDown.isDown;
                        win32_OutputDebug("\n");
					}
					else if(VKCode == 'D')
					{
						//keyboardController->dPadRight.isDown = !keyboardController->dPadRight.isDown;
					}
					else if(VKCode == 'Q')
					{
					}
					else if(VKCode == 'E')
					{
					}
					else if(VKCode == VK_UP)
					{
					}
					else if(VKCode == VK_LEFT)
					{
					}
					else if(VKCode == VK_DOWN)
					{
					}
					else if(VKCode == VK_RIGHT)
					{
					}
					else if(VKCode == VK_ESCAPE)
					{
						win32_OutputDebug("ESCAPE: ");
						if(isDown)
						{
							win32_OutputDebug("isDown ");
						}
						if(wasDown)
						{
							win32_OutputDebug("wasDown");
						}
						win32_OutputDebug("\n");
					}
					else if(VKCode == VK_SPACE)
					{
					}
				}
				
				b32 AltKeyWasDown = (message.lParam & (1 << 29));
				if((VKCode == VK_F4) && AltKeyWasDown)
				{
					globalRunning = false;
				}				
			} break;
			
			default:
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			} break;
		}
	}
}


inline s64
win32_GetWallClock(void) {
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return (s64)result.QuadPart;
}

inline f32
win32_GetSecondsElapsed(s64 start, s64 end) {
	f32 result = (f32)(end - start) / (f32)globalPerfCountFrequency;
	return result;
}

internal u32
Main(HINSTANCE instance)
{
	
	b32 sleepIsGranular = timeBeginPeriod(1); //NOTE(joshua): Set Sleep() millisecond granularity to 1 millisecond
#if CRT==ENABLED
	//DirectInput Testing
	char bufboi[1024];
	for(f32 i = -8.0f; i <= 8.0f; i+=0.01f)
	{
		f32 a = (f32)sin(i*PI32);
		f32 b = SINE_FUNC(i*PI32);
		f32 c = b - a;
		
		sprintf(bufboi, "i:: %f | sin: %f | mySine: %f | error: %f\n", i, a, b, c);
		win32_OutputDebug(bufboi);
	}
#endif
	LPCDIDEVICEINSTANCE controller = 0;
	
	IDirectInput8* DI8_interfacePointer = 0;
	if(DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8A, &(void*)DI8_interfacePointer, NULL) == DI_OK)
	{
		win32_OutputDebug("DirectInput8Create Success\n");
		
		HRESULT enumDevicesResult = DI8_interfacePointer->EnumDevices(DI8DEVCLASS_GAMECTRL, win32_GiveFirstController, &(void*)controller, DIEDFL_ATTACHEDONLY);
		if(enumDevicesResult == DI_OK)
		{
			win32_OutputDebug("DI8_interfacePointer->EnumDevices Success\n");
		} else {
			win32_OutputDebug("DI8_interfacePointer->EnumDevices Failed\n");
		}
	} else {
		win32_OutputDebug("DirectInput8Create Failed\n");
	}
	

	s32 monitorRefreshRate = 60;
	s32 gameTickRate = 120;
	s32 gameFrameRate = 30;
	f32 targetSecondsPerFrame = 1.0f / (f32)gameTickRate;
	
	WNDCLASS windowClass = {};
	windowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
	windowClass.lpfnWndProc = win32_MainWindowCallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = "AbyssEngineWindowClass";
	
	if(RegisterClass(&windowClass))
	{
        RECT desiredRect = {0};
        desiredRect.bottom = WINDOW_HEIGHT;
        desiredRect.right = WINDOW_WIDTH;
        AdjustWindowRect(&desiredRect, WINDOW_STYLE, false);
		HWND window = CreateWindow(windowClass.lpszClassName, "Abyss Engine",
											WINDOW_STYLE,
											10, 10, desiredRect.right-desiredRect.left, desiredRect.bottom-desiredRect.top,
											0, 0, instance, 0);
		
		if(window)
		{
			globalRunning = true;
			
			//Init Systems
			win32_XInput_LoadLibraryAndFunctions();
			
			win32_buffer_sound_output win32SoundOutput = {};
			win32SoundOutput.samplesPerSecond = 48000;
			win32SoundOutput.bytesPerSample = sizeof(s16)*2;
			win32SoundOutput.secondaryBufferSize = win32SoundOutput.samplesPerSecond*win32SoundOutput.bytesPerSample;
			win32SoundOutput.latencySampleCount = win32SoundOutput.samplesPerSecond / 15;
			
			win32_InitDirectSound(window, win32SoundOutput.samplesPerSecond, win32SoundOutput.secondaryBufferSize);
			win32_ClearSoundBuffer(&win32SoundOutput);
			globalSecondarySoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
			
			win32_buffer_video_output win32BufferVideoOutput = {};
            
            game_buffer_video_output intermediateBuffer = {};
            win32BufferVideoOutput.intermediateBuffer = &intermediateBuffer;
            

			game_buffer_video_output bufferVideoOutput = {};
			win32_ResizeVideoBuffers(&win32BufferVideoOutput, &bufferVideoOutput, 656, 519);

			s16* samples = (s16*)VirtualAlloc(0, win32SoundOutput.secondaryBufferSize,
														 MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			
			
			
			
			game_buffer_sound_output bufferSoundOutput = {};
			bufferSoundOutput.samplesPerSecond = win32SoundOutput.samplesPerSecond;
			bufferSoundOutput.samples = samples;
			
			game_input gameInput[2] = {};
			game_input* oldInputState = &gameInput[0];
			game_input* newInputState = &gameInput[1];
			
#if RELEASE_MODE==RELEASEMODE_INTERNAL
			LPVOID baseAddress = (LPVOID)Terabytes(1);
#else
			LPVOID baseAddress = 0;
#endif
			
			game_memory gameMemory = {};
			gameMemory.permanentStorageSize = Megabytes(128);
			gameMemory.transientStorageSize = Gigabytes(1);
			u64 totalSize = gameMemory.permanentStorageSize + gameMemory.transientStorageSize;
			gameMemory.permanentStorage = VirtualAlloc(baseAddress, totalSize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
			
			gameMemory.transientStorage = ((u8*)gameMemory.permanentStorage + gameMemory.permanentStorageSize);
			
			if(!samples || !gameMemory.permanentStorage || !gameMemory.transientStorage) {
				win32_OutputDebug("PANIC!\n");
				//exit(1);
			}
			
			//D3D TESTING
			
			win32_D3D11_Init(window);
			win32_D3D11_InitGraphics();
			
			HDC deviceContext = GetDC(window);
			
			
			{
				LARGE_INTEGER pcf;
				QueryPerformanceFrequency(&pcf);
				globalPerfCountFrequency = pcf.QuadPart;
			}
			
			s64 lastCounter = win32_GetWallClock();
			u64 lastCycleCount = __rdtsc();
			
			while(globalRunning)
			{
				win32_ProcessPendingMessages(window, newInputState, oldInputState, &bufferVideoOutput, &win32BufferVideoOutput);
				
				win32_XInput_PollControllers(oldInputState, newInputState);
				
				GameUpdateAndRender(&gameMemory, newInputState, &bufferVideoOutput, &bufferSoundOutput);
				
				//Output Game Audio To Platform
				win32_OutputAudio(&win32SoundOutput, &bufferSoundOutput);
				
				//Output Game Video To Platform
				win32_rect_dim clientDim = win32_GetClientDimensions(window);
				//win32_BlitBackbufferToClient(deviceContext, &win32BufferVideoOutput, clientDim.width, clientDim.height);
				
				//Swap InputState pointers
				game_input* temp = newInputState;
				newInputState = oldInputState;
				oldInputState = temp;
				
				
				u64 endCycleCount = __rdtsc();
				s64 endCounter = win32_GetWallClock();
				
				u64 cyclesElapsed = endCycleCount - lastCycleCount;
				
				f32 secondsElapsedForWork =  win32_GetSecondsElapsed(lastCounter, endCounter);
				
				f32 secondsElapsedForFrame = secondsElapsedForWork;
				while(secondsElapsedForFrame < targetSecondsPerFrame)
				{
					if(sleepIsGranular == TIMERR_NOERROR) { //NOTE(joshua): Only sleep if there is 1 millisecond granularity. 
						s32 sleepMS = maths::Clamp((s32)(targetSecondsPerFrame - secondsElapsedForFrame) - 1, 0, (s32)(targetSecondsPerFrame - 0.9));
						Sleep(sleepMS);
					}
					endCounter = win32_GetWallClock();
					secondsElapsedForFrame = win32_GetSecondsElapsed(lastCounter, win32_GetWallClock());
				}
				//TODO(joshua): MAKE SURE TO RENDER HERE _AFTER_ WAITING, NOT BEFORE.
				f64 msPerFrame = (1000.0f * win32_GetSecondsElapsed(lastCounter, win32_GetWallClock()));
				f64 FPS = 1.0f / win32_GetSecondsElapsed(lastCounter, win32_GetWallClock());
				f64 MCPF = ((f64)cyclesElapsed / (1000.0f * 1000.0f));
				
#if CRT==ENABLED
				char buffer[256];
				sprintf_s(buffer, sizeof(buffer), "%.02fms/f,  %.02ff/s,  %.02fmc/f\n", msPerFrame, FPS, MCPF);
				win32_OutputDebug(buffer);
				
#endif
				
				//win32_D3D11_RenderFrame();
				lastCounter = win32_GetWallClock();
				lastCycleCount = endCycleCount;
			}
		} else {
			//TODO(jdolman): Log CreateWindowEx failed!
		}
	} else {
		//TODO(jdolman): Log RegisterClass failed!
	}
	
	win32_D3D11_Clean();
	
	return(0);
}

#if CRT==DISABLED
void WinMainCRTStartup() 
{
	u32 result = Main(GetModuleHandle(NULL));
	ExitProcess(result);
}
#else
int CALLBACK WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdShow)
{
	u32 result = Main(instance);
	return (int)result;
}
#endif