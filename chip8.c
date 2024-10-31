#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "SDL.h"

typedef struct {
	SDL_Window* window;
	SDL_Renderer* render;
} sdl_t;


typedef struct {
	uint32_t window_width;
	uint32_t window_height;
	uint32_t for_color;
	uint32_t back_color;
} config_t;

//sdl congiguration from command and wudth and window_height
bool config_sdl(config_t* config) {
	config->window_width = 64;
	config->window_height = 32;
	config->for_color = 0xFFFFFFF;
	config->back_color = 0x0000000;
	return true;
}

//set wundow clear and color
void color_sdl(sdl_t sdl,config_t config) {
	const uint8_t r = (config.back_color >> 24)& 0xFF;
	const uint8_t g = (config.back_color >> 16)& 0xFF;
	const uint8_t b = (config.back_color >> 8)& 0xFF;
	const uint8_t a = (config.back_color >> 0)& 0xFF;

	SDL_SetRenderDrawColor(sdl.render,r,g,b,a);
	SDL_RenderClear(sdl.render);
}

//sdl update window 
void update_sdl(sdl_t sdl) {
  SDL_RenderPresent(sdl.render);
}
//sdl initialization
bool init_sdl(sdl_t* sdl,const config_t config) {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)!=0) {
		SDL_Log("could not initialize sdl sub systems : %s ",SDL_GetError());
		return false;
	}
	sdl->window = SDL_CreateWindow("chip8 emu",SDL_WINDOWPOS_CENTERED,                                          SDL_WINDOWPOS_CENTERED,config.window_width,                                 config.window_height,false);

	if(!sdl->window) {
		SDL_Log("could not craete a window %s ",SDL_GetError());
		return false;
	}

	sdl->render = SDL_CreateRenderer(sdl->window, -1,SDL_RENDERER_ACCELERATED);

	if(!sdl->render) {
		SDL_Log("could not create renderer %s ",SDL_GetError());
	}
	return true;
}

//sdl cleanup
void cleanup_sdl(sdl_t* sdl) {
	SDL_DestroyRenderer(sdl->render);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}


int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	sdl_t sdl = {0};
	config_t config = {0};
	//initialize emu congig
	if(!config_sdl(&config)) return -1;

	//start sdl
	if(!init_sdl(&sdl,config)) return -1;

	//set the initial screen
	color_sdl(sdl,config);
	//main emu loop
	while(true) {
    SDL_Delay(16);//delay for 16 millisecinds around 60 fps
		update_sdl(sdl);
	}

	//stop
	cleanup_sdl(&sdl);
	return 0;
}
