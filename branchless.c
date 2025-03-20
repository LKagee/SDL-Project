#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#define WINDOW_NAME "Window"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 80
struct Game {
	SDL_Window *window;
	SDL_Renderer *renderer;
  TTF_Font *ttf_font;
  SDL_Color text_color;
  SDL_Rect sdl_rectum;
  SDL_Texture *text_image;
  SDL_Texture *sprite_image;
  SDL_Rect sprite_rect;
  int text_yvol;
  int text_xvol;
  int sprite_vol;
  const Uint8 *keystate;
};

bool sdl_initialize(struct Game *game);
bool sdl_loadmedia(struct Game *game);
void sdl_cleanup(struct Game *game, int exit_status);
void update_text(struct Game *game);
void update_sprite(struct Game *game);

int main()
{
	struct Game game = {
		.window = NULL,
		.renderer = NULL,
    .ttf_font = NULL,
    .text_color = {255, 255, 255, 255},
    .sdl_rectum = {0, 0, 0, 0},
    .text_image = NULL,
    .sprite_image = NULL,
    .sprite_rect = {0, 0, 0, 0},
    .text_yvol = 3,
    .text_xvol = 3,
    .sprite_vol = 5,
    .keystate = SDL_GetKeyboardState(NULL),
	};
	
	if(sdl_initialize(&game)) {
		sdl_cleanup(&game, EXIT_FAILURE);
		return 1;
	}

  if(sdl_loadmedia(&game)) {
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

            default: break;
            }

        default: break;
            }
        }
    }
  update_text(&game);
  update_sprite(&game);
	SDL_RenderClear(game.renderer);
  SDL_RenderCopy(game.renderer, game.text_image, NULL, &game.sdl_rectum);
  SDL_RenderCopy(game.renderer, game.sprite_image, NULL, &game.sprite_rect);
	SDL_RenderPresent(game.renderer);
	SDL_Delay(16);
    }
  } 







void sdl_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyTexture(game->sprite_image);
  SDL_DestroyTexture(game->text_image);
  TTF_CloseFont(game->ttf_font);
	SDL_DestroyWindow(game->window);
	SDL_DestroyRenderer(game->renderer);
  IMG_Quit();
  TTF_Quit();
	SDL_Quit();
	exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Error initializing: %s\n", SDL_GetError() );
	return true;
}

game->window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

int img_init = IMG_Init(IMAGE_FLAGS);
      if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
    fprintf(stderr, "Error initializing image: %s", IMG_GetError() );
    return true;
  }

SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

srand((unsigned)time(NULL));

SDL_SetWindowMinimumSize(game->window, 800, 600);

return false;
}

bool sdl_loadmedia(struct Game *game) {
  game->ttf_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
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
  game->text_image = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);
    if(!game->text_image) {
      fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError() );
      return true;
  }

    game->sprite_image = IMG_LoadTexture(game->renderer, "images/image.png");
      if(!game->sprite_image) {
        fprintf(stderr, "Error creating sprite texture: %s\n", IMG_GetError() );
        return true;
  }

  if(SDL_QueryTexture(game->sprite_image, NULL, NULL, &game->sprite_rect.w, &game->sprite_rect.h)) {
    fprintf(stderr, "Error querying: %s", SDL_GetError() );
    return true;
  }


  return false;
}

void update_text(struct Game *game) {
  game->sdl_rectum.x += game->text_xvol;
  game->sdl_rectum.y += game->text_yvol;
  if(game->sdl_rectum.x + game->sdl_rectum.w > WINDOW_WIDTH) {
      game->text_xvol = -3;
  }
  
  if(game->sdl_rectum.x < 0) {
      game->text_xvol = 3;
  }

  if(game->sdl_rectum.y + game->sdl_rectum.h > WINDOW_HEIGHT) {
      game->text_yvol = -3;
  }

  if(game->sdl_rectum.y < 0) {
      game->text_yvol = 3;
  }
}

void update_sprite(struct Game *game) {
  if(game->keystate[SDL_SCANCODE_A] || game->keystate[SDL_SCANCODE_LEFT]) {
      game->sprite_rect.x -= game->sprite_vol;
  }
  
  if(game->keystate[SDL_SCANCODE_D] || game->keystate[SDL_SCANCODE_RIGHT]) {
      game->sprite_rect.x += game->sprite_vol;
  }
  
  if(game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) {
      game->sprite_rect.y -= game->sprite_vol;
  }

  if(game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) {
      game->sprite_rect.y += game->sprite_vol;
  }

}
