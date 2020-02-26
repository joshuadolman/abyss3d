@echo off
echo Running "Abyss3d" build.bat
REM set STARTTIME=%TIME%

REM === WORK AFTER THIS LINE ===

del *.pdb 2> nul


REM =================
REM --- Filenames ---
REM =================
set UNITYBUILDFILE=win32_main.cpp
set EXEFILE=abyss3d

REM ========================
REM --- Compiler Toggles ---
REM ========================
set RELEASE_TYPE=INTERNAL
REM (Final Shipping: RELEASE, Debug: INTERNAL)

set OS=OS_WINDOWS
REM OS Settings (Windows: OS_WINDOWS, Linux: OS_LINUX, mac: OS_MACOSX)
set CRT=CRT=1
REM sets whether we are using the CRT or not (enabled: CRT=1, disabled: CRT=0)


set DEBUG_OUTPUT=DEBUG_OUTPUT=1
REM sets whether we are outputting some sort of debug stream (enabled: DEBUG_OUTPUT=1, disabled: DEBUG_OUTPUT=0)
set ASSERTS=ABYSS_ASSERTS=1
REM sets whether we are using asserts or not (enabled: ASSERTS=1, disabled: ASSERTS=0)
set RELEASE=RELEASE_MODE=1
REM DONT SET MANUALLY - sets what release mode we are in for the compiler (Final Shipping: 0, Debug: 1)

set OPTIMISER_LEVEL=d
REM optimisation level (debug: d, fast: 2)

 IF "%RELEASE_TYPE%"=="INTERNAL" (
	set DEBUG_OUTPUT=DEBUG_OUTPUT=1
	set ASSERTS="ABYSS_ASSERTS=1"
	set RELEASE="RELEASE_MODE=1"
	set OPTIMISER_LEVEL=d
)

IF "%RELEASE_TYPE%"=="RELEASE" (
	set DEBUG_OUTPUT=DEBUG_OUTPUT=0
	set ASSERTS="ABYSS_ASSERTS=0"
	set RELEASE="RELEASE_MODE=0"
	set OPTIMISER_LEVEL=2
)

set DEFINES=-D%OS%=1 -D%CRT% -D%DEBUG_OUTPUT% -D%ASSERTS% -D%RELEASE%



REM =========================
REM --- Disabled Warnings ---
REM =========================
set DISABLEDWARNINGS=-wd4100 -wd4189 -wd4710 -wd4668 -wd4711 -wd4820 -wd4505 -wd4211 -wd4201 -wd4996 -wd4005 -wd4838
REM 4100 - "Unreferenced formal parameter" (Unused function parameter)
REM 4189 - "Local Variable is Initialized but not referenced" (Unused Variables?)
REM 4711 - "xx selected for automatic inline expansion" (Function auto-inlined?)
REM 4820 - "xx bytes padding added after data member xx" (Struct padding warning)
REM 4505 - "xx unreferenced local function has been removed" (Unused 'internal' function?)
REM 4211 - redefined extern to static (thinks we have an internal function that is supposed to be external)
REM 4201 - "nonstandard extension used: nameless struct/union" (anonymous struct + union may not be supported on every c++ compiler)
REM 4996 - Decprecated stuff
REM 4005 - Macro Redefinition
REM 4838 - conversion from unsigned int to INT requires a narrowing conversion (for xnamath.h ugggh)

REM ================
REM --- Compiler ---
REM ================
set STACKSIZE=10485760
set COMPILERFLAGS=-F%STACKSIZE% -MT -O%OPTIMISER_LEVEL% -Oi -GS- -Gs9999999 -Gm- -GR- -EHa -utf-8 -Oi -Z7 -nologo -FC -W4 -WX %DISABLEDWARNINGS% -I"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include"

IF "%CRT%"=="CRT=0" (
	set CRT_LIBS=kernel32.lib
	set CRT_LINKERFLAGS=-nodefaultlib
)

IF "%CRT%"=="CRT=1" (
	set "CRT_LIBS="
	set "CRT_LINKERFLAGS="
)

set DINPUTLIBS=dinput8.lib dxguid.lib
set D3DLIBS=d3d11.lib d3dx11.lib d3dx10.lib
set LIBS=user32.lib gdi32.lib Winmm.lib %CRT_LIBS% %DINPUTLIBS% %D3DLIBS%
set LINKERFLAGS=-opt:ref %CRT_LINKERFLAGS% -subsystem:windows -stack:%STACKSIZE%,%STACKSIZE% %LIBS% -LIBPATH:"C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64"

cl %DEFINES% %COMPILERFLAGS% -Fm:%EXEFILE%.map -Fe:%EXEFILE%.exe %UNITYBUILDFILE% /link %LINKERFLAGS%


del *.obj 2> nul

REM === WORK BEFORE THIS LINE ===

REM set ENDTIME=%TIME%

REM set /A STARTTIME=(1%STARTTIME:~0,2%-100)*360000 + (1%STARTTIME:~3,2%-100)*6000 + (1%STARTTIME:~6,2%-100)*100 + (1%STARTTIME:~9,2%-100)
REM set /A ENDTIME=(1%ENDTIME:~0,2%-100)*360000 + (1%ENDTIME:~3,2%-100)*6000 + (1%ENDTIME:~6,2%-100)*100 + (1%ENDTIME:~9,2%-100)

REM set /A DURATION=%ENDTIME%-%STARTTIME%
echo.
REM echo Compile time: %DURATION% centiseconds 