#include "chip8.h"

chip8 Chip8;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
//Mix_Music *wave = NULL;

//#define WAV_PATH "beep.wav"

const Uint32 fps = 60;
const Uint32 freq = 400;
const Uint32 minframetime = 1000 / fps;

static const uint8_t SCREEN_WIDTH = 64;
static const uint8_t SCREEN_HEIGHT = 32;
//static const uint8_t PIXEL_SIZE = 12;

// Temporary pixel buffer  CURRENTLY DONT USE MIGHT UPDATE LATER
// uint32_t pixels[2048];

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
	// Initialize SDL subsystems(video and mixer) 
	// TODO: Sound
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, NULL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	
	//Initialize SDL_mixer
	/*if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) 
		return -1; 
	wave = Mix_LoadMUS(WAV_PATH);
	if (wave == NULL)
		return -1;
	*/
	return 0;
}
/*
void play_audio(unsigned char sound_timer)
{
	if (sound_timer > 0)
	{
		if (Mix_PlayingMusic() == 0)
			Mix_PlayMusic(wave, -1);
	}
	else
	{
		if (Mix_PlayingMusic() == 1)
			Mix_HaltMusic();
	}
}
*/
int main(int argc, char **argv)
{

	if (argc < 2)
	{
		printf("Usage: Chip8.exe chip8application\n\n");
		return 1;
	}

	init_SDL();

	Uint32 start_time;
	Uint32 last_time;
	Uint32 elapsed_time;

	// Load game
load:
	if (!Chip8.loadApplication(argv[1]))
		return 1;

	SDL_Event e;
	bool running = true;
	float cycles = 0;
	last_time = SDL_GetTicks();

	// Emulation loop
	while (running) {
		start_time = SDL_GetTicks();
		elapsed_time = start_time - last_time;

		// Process SDL events
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

		Chip8.emulateCycle();
		//play_audio(Chip8.sound_timer);

		// If draw occurred, redraw SDL screen
		if (Chip8.drawFlag)
		{
			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// Draw screen
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_Rect *destRect = new SDL_Rect;
			destRect->x = 0;
			destRect->y = 0;
			destRect->w = 8;
			destRect->h = 8;


			for (int y = 0; y < 32; y++)
			{
				for (int x = 0; x < 64; x++)
				{
					if (Chip8.gfx[(y * 64) + x] == 1)
					{
						destRect->x = x * 8;
						destRect->y = y * 8;

						SDL_RenderFillRect(renderer, destRect);
					}
				}
			}

			delete destRect;

			SDL_RenderPresent(renderer);
			Chip8.drawFlag = false;
		}
		// Limit frame rate
		//if (SDL_GetTicks() - start_time < minframetime)
		//	SDL_Delay(minframetime - (SDL_GetTicks() - start_time));

		last_time = start_time;
	}

	SDL_Quit();
}