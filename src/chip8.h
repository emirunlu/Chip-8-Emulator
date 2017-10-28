#include "SDL.h"
#include <SDL_main.h>
#include <SDL_render.h>
#include <SDL_opengl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <thread>
#include "stdint.h"

#pragma warning (disable : 4996)
class chip8 {
public:
	chip8();
	~chip8();

	bool drawFlag;

	void emulateCycle();
	bool loadApplication(const char * filename);

	// Chip8
	unsigned char  gfx[64 * 32];	// Total amount of pixels: 2048
	unsigned char  key[16];

private:
	unsigned short pc;				// Program counter
	unsigned short opcode;			// Current opcode
	unsigned short I;				// Index register
	unsigned short sp;				// Stack pointer

	unsigned char  V[16];			// V-regs (V0-VF)
	unsigned short stack[16];		// Stack (16 levels)
	unsigned char  memory[4096];	// Memory (size = 4k)		

	unsigned char  delay_timer;		// Delay timer
	unsigned char  sound_timer;		// Sound timer		

	void init();
};
