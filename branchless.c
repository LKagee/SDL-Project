#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <time.h>
#define WINDOW_NAME "Window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TEXT_SIZE 80
struct Game {
	SDL_Window *window;
	SDL_Renderer *renderer;
  TTF_Font *ttf_font;
  SDL_Color *text_color;
  SDL_Rect *sdl_rectum;
  SDL_Texture *text_image;
};

bool sdl_initialize(struct Game *game);
bool sdl_loadmedia(struct Game *game);
void sdl_cleanup(struct Game *game, int exit_status);

int main()
{
	struct Game game = {
		.window = NULL,
		.renderer = NULL,
    .surface = NULL,
    .ttf_font = NULL,
    .text_color = {255, 255, 255, 255},
    .sdl_rectum = {0, 0, 0, 0},
    .text_image = NULL,
	};
	
	if(sdl_initialize(&game)) {
		sdl_cleanup(&game, EXIT_FAILURE);
		return 1;
	}
  SDL_SetWindowBrightness(game.window, 1.0);

	while(true) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT: {
				sdl_cleanup(&game, EXIT_SUCCESS); break;}
			case SDL_KEYDOWN: {
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: {
							sdl_cleanup(&game, EXIT_SUCCESS); break;}
            case SDLK_TAB: {
              sdl_cleanup(&game, EXIT_SUCCESS); break;}
            case SDLK_SPACE: {
              SDL_SetRenderDrawColor(game.renderer, rand() % 256, rand() % 256, rand() % 256, 255); break;}
            case SDLK_w: {
             SDL_MaximizeWindow(game.window); break;}

            case SDLK_s: {
             SDL_MinimizeWindow(game.window); break;}
            }
            }
}
  
	SDL_RenderClear(game.renderer);
	SDL_RenderPresent(game.renderer);
	SDL_Delay(16);
    }
  } 
}






void sdl_cleanup(struct Game *game, int exit_status) {
	SDL_DestroyWindow(game->window);
	SDL_DestroyRenderer(game->renderer);
	SDL_Quit();
	exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Error initializing: %s\n", SDL_GetError() );
	return true;
}

game->window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if(!game->window) {
		fprintf(stderr, "Error creating window: %s\n", SDL_GetError() );
	return true;
}

game->renderer = SDL_CreateRenderer(game->window, -1, 0);
	if(!game->renderer) {
		fprintf(stderr, "Error creating renderer: %s", SDL_GetError() );
	return true;
}

if(TTF_Init()) {
    fprintf(stderr, "Error sucking cock: %s", TTF_GetError() );
    return true;
  }

SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

srand((unsigned)time(NULL));

SDL_SetWindowMinimumSize(game->window, 800, 600);

return false;
}

bool sdl_loadmedia(struct Game *game) {
  game->ttf_font = TTF_OpenFont(tbd, TEXT_SIZE);
    if(!game->ttf_font) {
      fprintf(stderr, "Error creating font: %s", TTF_GetError() );
      return true;
  }

  SDL_Surface *surface = 
    TTF_RenderText_Blended(game->ttf_font, "SDL", game->text_color);
      if(!surface) {
        fprintf(stderr, "Error rendering text: %s", TTF_GetError() );
        return true;
  }

  game->sdl_rectum.w = surface->w;
  game->sdl_rectum.h = surface->h;
}
