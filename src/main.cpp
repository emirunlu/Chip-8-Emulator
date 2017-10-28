#include "chip8.h"

// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

chip8 Chip8;
int modifier = 10;
bool quit = false;

// Window size
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

typedef unsigned __int8 u8;
u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

const unsigned char NCOLORS = 2;
Uint32 palette[NCOLORS];

SDL_Window *window = nullptr;
SDL_Texture *sdlTexture = nullptr;
SDL_Renderer *renderer = nullptr;

// Temporary pixel buffer
uint32_t pixels[2048];

// Chip8 Keypad
uint8_t keymap[16] = {
	SDLK_x,
	SDLK_1,
	SDLK_2,
	SDLK_3,
	SDLK_q,
	SDLK_w,
	SDLK_e,
	SDLK_a,
	SDLK_s,
	SDLK_d,
	SDLK_z,
	SDLK_c,
	SDLK_4,
	SDLK_r,
	SDLK_f,
	SDLK_v,
};

int init_SDL()
{
	//Initialize all SDL subsystems
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Chip8 Emulator", 100, 100, display_width, display_height, SDL_WINDOW_SHOWN);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_RenderSetLogicalSize(renderer, display_width, display_height);

	SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		64, 32);

	return 0;
}

void stop_SDL()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: Chip8.exe chip8application\n\n");
		return 1;
	}

	init_SDL();

	// Load game
load:
	if (!Chip8.loadApplication(argv[1]))
		return 1;

	// Emulation loop
	while (true) {
		Chip8.emulateCycle();

		// Process SDL events
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) exit(0);

			// Process keydown events
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE)
					exit(0);

				if (e.key.keysym.sym == SDLK_F1)
					goto load;      // *gasp*, a goto statement!
									// Used to reset/reload ROM

				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) {
						Chip8.key[i] = 1;
					}
				}
			}
			// Process keyup events
			if (e.type == SDL_KEYUP) {
				for (int i = 0; i < 16; ++i) {
					if (e.key.keysym.sym == keymap[i]) {
						Chip8.key[i] = 0;
					}
				}
			}
		}

		// If draw occurred, redraw SDL screen
		if (Chip8.drawFlag) {
			Chip8.drawFlag = false;

			// Store pixels in temporary buffer
			for (int i = 0; i < 2048; ++i) {
				uint8_t pixel = Chip8.gfx[i];
				pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
			}
			// Update SDL texture
			SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));
			// Clear screen and render
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, sdlTexture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}

		// Sleep to slow down emulation speed
		std::this_thread::sleep_for(std::chrono::microseconds(1200));
		
	}

	stop_SDL();
	return 0;
}