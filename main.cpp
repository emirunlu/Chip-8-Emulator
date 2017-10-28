#include "chip8.h"

// Display size
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

chip8 Chip8;
int modifier = 10;
int oglIdx = -1;
bool quit = false;

// Window size
int display_width = SCREEN_WIDTH * modifier;
int display_height = SCREEN_HEIGHT * modifier;

void display();
void reshape_window(GLsizei w, GLsizei h);

typedef unsigned __int8 u8;
u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];
void setupTexture();
const unsigned char NCOLORS = 2;
Uint32 palette[NCOLORS];

SDL_Window *window = nullptr;
SDL_Surface *screen = nullptr;

// Chip8 Keypad
const Uint32 KEY_0 = SDL_SCANCODE_KP_0;
const Uint32 KEY_1 = SDL_SCANCODE_KP_1;
const Uint32 KEY_2 = SDL_SCANCODE_KP_2;
const Uint32 KEY_3 = SDL_SCANCODE_KP_3;
const Uint32 KEY_4 = SDL_SCANCODE_KP_4;
const Uint32 KEY_5 = SDL_SCANCODE_KP_5;
const Uint32 KEY_6 = SDL_SCANCODE_KP_6;
const Uint32 KEY_7 = SDL_SCANCODE_KP_7;
const Uint32 KEY_8 = SDL_SCANCODE_KP_8;
const Uint32 KEY_9 = SDL_SCANCODE_KP_9;
const Uint32 KEY_A = SDL_SCANCODE_Q;
const Uint32 KEY_B = SDL_SCANCODE_A;
const Uint32 KEY_C = SDL_SCANCODE_Z;
const Uint32 KEY_D = SDL_SCANCODE_W;
const Uint32 KEY_E = SDL_SCANCODE_S;
const Uint32 KEY_F = SDL_SCANCODE_X;

const Uint32 KEY_EXIT = SDLK_ESCAPE;

int init_SDL()
{
	//Initialize all SDL subsystems
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("Chip8 Emulator", 100, 100, display_width, display_height, SDL_WINDOW_SHOWN);

	screen = SDL_GetWindowSurface(window);

	return 0;
}

void setup_palette()
{
	palette[0] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	palette[1] = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
}

int setup_graphics()
{
	if (init_SDL())
		return 1;
	setup_palette();

	return 0;
}

void render_SDL(unsigned char gfx[])
{
	SDL_Rect pixel = {0, 0, modifier, modifier};

	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		pixel.x = i * modifier;
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			pixel.y = j * modifier;
			SDL_FillRect(screen, &pixel, palette[gfx[i*j]]);
		}
	}
	SDL_UpdateWindowSurface(window);
}

void stop_SDL()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void update_key_reg(unsigned char key[])
{
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	key[0x0] = keystates[KEY_0];
	key[0x1] = keystates[KEY_1];
	key[0x2] = keystates[KEY_2];
	key[0x3] = keystates[KEY_3];
	key[0x4] = keystates[KEY_4];
	key[0x5] = keystates[KEY_5];
	key[0x6] = keystates[KEY_6];
	key[0x7] = keystates[KEY_7];
	key[0x8] = keystates[KEY_8];
	key[0x9] = keystates[KEY_9];
	key[0xA] = keystates[KEY_A];
	key[0xB] = keystates[KEY_B];
	key[0xC] = keystates[KEY_C];
	key[0xD] = keystates[KEY_D];
	key[0xE] = keystates[KEY_E];
	key[0xF] = keystates[KEY_F];
}

int process_event(SDL_Event *e, chip8* Chip8)
{
	if (e->type == SDL_QUIT)
		return 1;
	if (!e->key.repeat)
	{
		if (e->type == SDL_KEYDOWN)
		{
			switch (e->key.keysym.sym)
			{
				/*
				case KEY_A:
					std::cout << "key Z pressed" << std::endl;
				break;
				*/
			case KEY_EXIT:
				return 1;
				break;
			default:
				break;
			}
		}
		/*
		if (e->type == SDL_KEYUP)
		{
			switch (e->key.keysym.sym)
			{
				case KEY_A:
					std::cout << "key Z unpressed" << std::endl;
				break;
				default:
				break;
			}
		}
		*/
		if (e->type == SDL_KEYUP || e->type == SDL_KEYDOWN)
			update_key_reg(Chip8->key);
	}
	return 0;
}


int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: Chip8.exe chip8application\n\n");
		return 1;
	}

	// Load game
	if (!Chip8.loadApplication(argv[1]))
		return 1;

	// Setup OpenGL
	SDL_Event e;
	setup_graphics();
	while (1) {
		while (SDL_PollEvent(&e))
		{
			if (process_event(&e, &Chip8))
				quit = true;
		}

		render_SDL(Chip8.gfx);

	}
	

	stop_SDL();
	return 0;
}
