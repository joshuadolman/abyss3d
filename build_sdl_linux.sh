#!/bin/bash
# =================
# --- Filenames ---
# =================
UNITYBUILDFILE=sdl_main.cpp
EXEFILE=abyss_sdl_linux

# ========================
# --- Compiler Toggles ---
# ========================
OS="OS_LINUX"
CRT="1"

RELEASE_TYPE="INTERNAL"
#NOT YET USED!

DEBUG_OUTPUT="DEBUG_OUTPUT"
ASSERTS="ABYSS_ASSERTS=1"
RELEASE="RELEASE_MODE=1"

DEFINES="-D$OS -D$DEBUG_OUTPUT -D$ASSERTS"

OPTIMISER_LEVEL=0
# opimisation level (compilation speed: 0, execution speed: 2)

SDL_LIBS="-lm -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer"
LIBS="$SDL_LIBS"

gcc -g -O$OPTIMISER_LEVEL $DEFINES $UNITYBUILDFILE -o $EXEFILE $LIBS