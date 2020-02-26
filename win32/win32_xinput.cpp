#ifndef WIN32_XINPUT_CPP
#define WIN32_XINPUT_CPP

//NOTE(jdolman): XInputGetState
#define XINPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_GET_STATE(xinput_get_state);
XINPUT_GET_STATE(XInputGetState__STUB)
{
	return(ERROR_DEVICE_NOT_CONNECTED);
}
global xinput_get_state* XInputGetState_ = XInputGetState__STUB;
#define XInputGetState XInputGetState_

//NOTE(jdolman): XInputSetState
#define XINPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef XINPUT_SET_STATE(xinput_set_state);
XINPUT_SET_STATE(XInputSetState__STUB)
{
	return(ERROR_DEVICE_NOT_CONNECTED);
}
global xinput_set_state* XInputSetState_ = XInputSetState__STUB;
#define XInputSetState XInputSetState_

internal void
win32_XInput_LoadLibraryAndFunctions()
{
	HMODULE XInputLib = LoadLibrary("xinput1_4.dll");
	
	if(!XInputLib)
	{
		XInputLib = LoadLibrary("xinput1_3.dll");
	}
	if(!XInputLib)
	{
		XInputLib = LoadLibrary("xinput9_1_0.dll");
	}
	
	if(XInputLib)
	{
		XInputGetState = (xinput_get_state*)GetProcAddress(XInputLib, "XInputGetState");
		XInputSetState = (xinput_set_state*)GetProcAddress(XInputLib, "XInputSetState");
	}
}

internal void
win32_XInput_ProcessButton(WORD XInputButtons, WORD XInputButton, game_button_state* oldState, game_button_state* newState)
{
	if(XInputButtons && XInputButton)
	{
		if(oldState->isDown==FALSE)
		{
			newState->halfTransitions+=1;
		}
		newState->isDown=TRUE;
	} else {
		if(oldState->isDown==TRUE)
		{
			newState->halfTransitions-=1;
			if(newState->halfTransitions<0)
			{
				newState->halfTransitions=0;
			}
			newState->isDown=FALSE;
		}
	}
}

internal f32
win32_XInput_NormalizeStickAxis(s16 stick)
{
	f32 val;
	if(stick < 0)
	{
		val=0;
		val = (f32)stick / 32678.0f;
	} else {
		val = (f32)stick / 32677.0f;
	}
	return val; 
}

internal void
win32_XInput_PollControllers(game_input* oldInputState, game_input* newInputState)
{
	u32 maxControllerCount = XUSER_MAX_COUNT;
	
    /*
	if (maxControllerCount > ArrayCount(gameInput[0].controllers))
	{
		maxControllerCount = ArrayCount(gameInput[0].controllers);
	}
	*/
    game_controller_input* oldController = 0;
    game_controller_input* newController = 0;

	for(DWORD i=0; i < maxControllerCount; i++)
	{
		XINPUT_STATE controllerState;
		if(XInputGetState(i, &controllerState) == ERROR_SUCCESS)
		{
			oldController = &oldInputState->controllers[i];
			newController = &newInputState->controllers[i];
			XINPUT_GAMEPAD* gamepad = &controllerState.Gamepad;
			
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_A, &oldController->buttonA, &newController->buttonA);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_B, &oldController->buttonB, &newController->buttonB);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_X, &oldController->buttonX, &newController->buttonX);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_Y, &oldController->buttonY, &newController->buttonY);
			
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_DPAD_UP, &oldController->dPadUp, &newController->dPadUp);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_DPAD_DOWN, &oldController->dPadDown, &newController->dPadDown);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_DPAD_LEFT, &oldController->dPadLeft, &newController->dPadLeft);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_DPAD_RIGHT, &oldController->dPadRight, &newController->dPadRight);
			
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_START, &oldController->start, &newController->start);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_BACK, &oldController->select, &newController->select);
			
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, &oldController->shoulderLeft, &newController->shoulderLeft);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER, &oldController->shoulderRight, &newController->shoulderRight);
			
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_LEFT_THUMB, &oldController->thumbRight, &newController->thumbRight);
			win32_XInput_ProcessButton(gamepad->wButtons, XINPUT_GAMEPAD_RIGHT_THUMB, &oldController->thumbRight, &newController->thumbRight);
			
			newController->stickLeftX.start = oldController->stickLeftX.end;
			newController->stickLeftY.start = oldController->stickLeftY.end;
			newController->stickRightX.start = oldController->stickRightX.end;
			newController->stickRightY.start = oldController->stickRightY.end;
			
			newController->stickLeftX.min = newController->stickLeftX.max = newController->stickLeftX.end = win32_XInput_NormalizeStickAxis(gamepad->sThumbLX);newController->stickLeftY.min = newController->stickLeftY.max = newController->stickLeftY.end = win32_XInput_NormalizeStickAxis(gamepad->sThumbLY);
			newController->stickRightX.min = newController->stickRightX.max = newController->stickRightX.end = win32_XInput_NormalizeStickAxis(gamepad->sThumbRX);
			newController->stickRightY.min = newController->stickRightY.max = newController->stickRightY.end = win32_XInput_NormalizeStickAxis(gamepad->sThumbRY);
			
			
			//
		} else {
			//NOTE(jdolman): Controller is unplugged
		}
	}
}

#endif //WIN32_ABYSS_CPP