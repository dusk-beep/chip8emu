#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "SDL.h"
#include "SDL_video.h"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* render;
} sdl_t;

typedef struct {
	uint32_t window_width;
	uint32_t window_height;
	uint32_t for_color;
	uint32_t back_color;
	uint32_t scale_factor;
} config_t;

typedef enum {
	QUIT,
	RUNNING,
	PAUSED,
} emulator_state_t;

typedef struct {
	emulator_state_t state;
} chip8_t;

//sdl congiguration from command and wudth and window_height
bool config_sdl(config_t* config)
{
	config->window_width = 64;
	config->window_height = 32;
	config->for_color = 0xFFFFFFFF;
	config->back_color = 0xFF0000FF;
	config->scale_factor = 15;
	return true;
}

//set wundow clear and color
void color_sdl(sdl_t sdl, config_t config)
{
	const uint8_t r = (config.back_color >> 24) & 0xFF;
	const uint8_t g = (config.back_color >> 16) & 0xFF;
	const uint8_t b = (config.back_color >> 8) & 0xFF;
	const uint8_t a = (config.back_color >> 0) & 0xFF;

	SDL_SetRenderDrawColor(sdl.render, r, g, b, a);
	SDL_RenderClear(sdl.render);
}

//handle user inputs
void handle_input(chip8_t* chip8)
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type) {
		case SDL_QUIT :
			chip8->state = QUIT; //the while loop will exit in main
			return; //quit application when cross pressed
			//add more keys later
		}
	}
}

//initialize the chip8
bool init_chip8(chip8_t* chip8)
{
	chip8->state = RUNNING;
	return true;
}

//sdl update window
void update_sdl(sdl_t sdl)
{
	SDL_RenderPresent(sdl.render);
}

//sdl initialization
bool init_sdl(sdl_t* sdl, const config_t config)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
		SDL_Log("could not initialize sdl sub systems : %s ", SDL_GetError());
		return false;
	}

	sdl->window = SDL_CreateWindow("chip8 emu", SDL_WINDOWPOS_CENTERED,
	                               SDL_WINDOWPOS_CENTERED, config.window_width * config.scale_factor,
	                               config.window_height * config.scale_factor, SDL_WINDOW_SHOWN);

	if(!sdl->window) {
		SDL_Log("could not craete a window %s ", SDL_GetError());
		return false;
	}

	sdl->render = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_SOFTWARE);

	if(!sdl->render) {
		SDL_Log("could not create renderer %s ", SDL_GetError());
	}

	return true;
}

//sdl cleanup
void cleanup_sdl(sdl_t* sdl)
{
	SDL_DestroyRenderer(sdl->render);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}


int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	sdl_t sdl = {0};
	config_t config = {0};

	//initialize emu congig
	if(!config_sdl(&config)) return -1;

	//sinitialize sdl
	if(!init_sdl(&sdl, config)) return -1;

	//initialize chip
	chip8_t chip8 = {0};

	if(!init_chip8(&chip8)) return -1;

	//set the initial screen
	color_sdl(sdl, config);

	while(chip8.state != QUIT) {
		handle_input(&chip8); //if state paused then continue todo
		SDL_Delay(16);
		update_sdl(sdl);
	}

	cleanup_sdl(&sdl);
	return 0;
}

