//NOTES TO SELF: 
//-Create ball to ball collision response
//-Create ball to rectangle collision response
//-Add sound effects and music
//-Maybe add damage notifier through sprite flash
//-Cleanup up this shitty code
//-Play with threads like how I play with myself

#include <math.h>
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
bool button_press = false;
int radii = 26;
int diameter = 52;
int distance;
struct Menu {
  SDL_Rect menu_dist;
  SDL_Rect menu_button[2];
  SDL_Texture *button_image;
};

struct Game {
	SDL_Window *window;
	SDL_Renderer *renderer;
  TTF_Font *ttf_font;
  SDL_Color text_color;
  SDL_Rect enemy2_rect;
  SDL_Rect enemy_rect;
  SDL_Texture *enemy2_image;
  SDL_Texture *text_image;
  SDL_Texture *sprite_image;
  SDL_Rect sprite_rect;
  int enemy2_yvol;
  int enemy2_xvol;
  int text_yvol;
  int text_xvol;
  int sprite_vol;
  const Uint8 *keystate;
};

bool sdl_initialize(struct Game *game, struct Menu *menu);
bool sdl_loadmedia(struct Game *game, struct Menu *menu);
void sdl_cleanup(struct Game *game, struct Menu *menu, int exit_status);
bool button_highlight(struct Menu *menu, SDL_Event event);
bool menu_nav(struct Menu *menu, SDL_Event event, int a);
void update_enemy1(struct Game *game);
void update_enemy2(struct Game *game);
void update_sprite(struct Game *game);
bool checkCollision(struct Game *game);
bool checkCollision2(struct Game *game);
bool checkCollision3(struct Game *game);


int main()
{
  struct Menu menu = {
    .menu_dist = {290, 500, 223, 100},
    .menu_button[0].x = 0,
    .menu_button[0].y = 0,
    .menu_button[0].w = 223,
    .menu_button[0].h = 100,

    .menu_button[1].x = 0,
    .menu_button[1].y = 100,
    .menu_button[1].w = 223,
    .menu_button[1].h = 100,

    .button_image = NULL,
  };

	struct Game game = {
		.window = NULL,
		.renderer = NULL,
    .ttf_font = NULL,
    .text_color = {255, 255, 255, 255},
    .enemy_rect = {0, 0, 0, 0},
    .enemy2_rect = {0, 0, 0, 0},
    .enemy2_image = NULL,
    .text_image = NULL,
    .sprite_image = NULL,
    .sprite_rect = {0, 0, 0, 0},
    .enemy2_yvol = 5,
    .enemy2_xvol = 5,
    .text_yvol = 3,
    .text_xvol = 3,
    .sprite_vol = 5,
    .keystate = SDL_GetKeyboardState(NULL),
	};
	
	if(sdl_initialize(&game, &menu)) {
		sdl_cleanup(&game, &menu, EXIT_FAILURE);
		return 1;
	}

  if(sdl_loadmedia(&game, &menu)) {
    sdl_cleanup(&game, &menu, EXIT_FAILURE);
    return 1;
  }

  SDL_SetWindowBrightness(game.window, 1.0);
  int a = 1;
  int i;
 menu_loop:

  while(a == 1) {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch(event.type) {
        case SDL_QUIT: {a++; break;}

        case SDL_MOUSEMOTION: {
          switch(button_highlight(&menu, event)) {
            case true: {i = 1; break;}

            case false: {i = 0; break;}
          }
          break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            switch(event.button.button) {
            case SDL_BUTTON_LEFT: {
              switch(menu_nav(&menu, event, a))
              case true: {a++;}
              default: break;
          break;}
        break;}
      }
    }
  }

    SDL_RenderClear(game.renderer);
    SDL_RenderCopy(game.renderer, menu.button_image, &menu.menu_button[i], &menu.menu_dist);
    SDL_RenderPresent(game.renderer);
    SDL_Delay(16);
  }

	while(true) {
    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT: {
				sdl_cleanup(&game, &menu, EXIT_SUCCESS); break;}
			case SDL_KEYDOWN: {
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE: {
							a = 1; goto menu_loop; break;}
            case SDLK_TAB: {
              sdl_cleanup(&game, &menu, EXIT_SUCCESS); break;}
            case SDLK_SPACE: {
              SDL_SetRenderDrawColor(game.renderer, rand() % 256, rand() % 256, rand() % 256, 255); break;}

            default: break;
            }
        }
        default: break;
            }
        }

  if(checkCollision2(&game)) {
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    }

  if(checkCollision(&game)) {
      SDL_SetRenderDrawColor(game.renderer, 0, 0, 255, 255);
    }

  update_enemy2(&game);
  update_enemy1(&game);
  update_sprite(&game);
	SDL_RenderClear(game.renderer);
  SDL_RenderCopy(game.renderer, game.text_image, NULL, &game.enemy_rect);
  SDL_RenderCopy(game.renderer, game.sprite_image, NULL, &game.sprite_rect);
  SDL_RenderCopy(game.renderer, game.enemy2_image, NULL, &game.enemy2_rect);
	SDL_RenderPresent(game.renderer);
	SDL_Delay(16);
  }
}

 







void sdl_cleanup(struct Game *game, struct Menu *menu, int exit_status) {
  SDL_DestroyTexture(menu->button_image);
  SDL_DestroyTexture(game->enemy2_image);
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

bool sdl_initialize(struct Game *game, struct Menu *menu) {
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

bool sdl_loadmedia(struct Game *game, struct Menu *menu) {
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

  game->enemy2_image = IMG_LoadTexture(game->renderer, "images/enemy2.png");
     if(!game->enemy2_image) {
       fprintf(stderr, "Error creating brother enemy texture: %s\n", IMG_GetError() );
       return true;
  }

  game->text_image = IMG_LoadTexture(game->renderer, "images/enemy.png");
  SDL_FreeSurface(surface);
    if(!game->text_image) {
      fprintf(stderr, "Error creating enemy texture: %s\n", IMG_GetError() );
      return true;
  }

  menu->button_image = IMG_LoadTexture(game->renderer, "images/button.png");
    if(!menu->button_image) {
      fprintf(stderr, "Error creating button texture: %s\n", IMG_GetError() );
      return true;
  }

  game->sprite_image = IMG_LoadTexture(game->renderer, "images/image.png");
      if(!game->sprite_image) {
        fprintf(stderr, "Error creating sprite texture: %s\n", IMG_GetError() );
        return true;
  }

  if(SDL_QueryTexture(game->enemy2_image, NULL, NULL, &game->enemy2_rect.w, &game->enemy2_rect.h)) {
     fprintf(stderr, "Error querying: %s", SDL_GetError() );
     return true;
  }

  if(SDL_QueryTexture(game->text_image, NULL, NULL, &game->enemy_rect.w, &game->enemy_rect.h)) {
    fprintf(stderr, "Error querying: %s", SDL_GetError() );
    return true;
  }

  if(SDL_QueryTexture(game->sprite_image, NULL, NULL, &game->sprite_rect.w, &game->sprite_rect.h)) {
    fprintf(stderr, "Error querying: %s", SDL_GetError() );
    return true;
  }

  if(SDL_QueryTexture(menu->button_image, NULL, NULL, &menu->menu_button[2].w, &menu->menu_button[2].h)) {
    fprintf(stderr, "Error querying: %s", SDL_GetError() );
    return true;
  }

  return false;
}

//revise this into ball to ball collision detection using radii of circles
bool checkCollision2(struct Game *game) {
  int ballAX = game->enemy_rect.x;
  int ballBX = game->enemy2_rect.x;
  int ballAY = game->enemy_rect.y;
  int ballBY = game->enemy2_rect.y;
  int XDist = ballBX - ballAX;
  int YDist = ballBY - ballAY;
  distance = sqrt((XDist * XDist) + (YDist * YDist));
  if(distance <= diameter) {
    return true;
  }

  return false;
}

//revise to be only for ball to rectangle collision detection
bool checkCollision(struct Game *game) {
  //I dont want to have to keep typing "game->enemy_rect" everytime I want their x, y, w, or h values.
  int cxRED = game->enemy_rect.x;
  int cyRED = game->enemy_rect.y;
  int cxBLUE = game->enemy2_rect.x;
  int cyBLUE = game->enemy2_rect.y;
  int rx = game->sprite_rect.x;
  int ry = game->sprite_rect.y;
  int rh = game->sprite_rect.h;
  int rw = game->sprite_rect.w;
  int testxRED = cxRED;
  int testyRED = cyRED;
  int testxBLUE = cxBLUE;
  int testyBLUE = cyBLUE;

  if(cxRED < rx) {
    testxRED = rx;
  } else if(cxRED > rx + rh) {
    testxRED = rx + rh;
  }

  if(cyRED < ry) {
    testyRED = ry;
  } else if(cyRED > ry + rw) {
    testyRED = ry + rw;
  }

  if(cxBLUE < rx) {
    testxBLUE = rx;
  } else if(cxBLUE > rx + rh) {
    testxBLUE = rx + rh;
  }

  if(cyBLUE < ry) {
    testyBLUE = ry;
  } else if(cyBLUE > ry + rw) {
    testyBLUE = ry + rw;
  }

  float distXRED = cxRED - testxRED;
  float distYRED = cyRED - testyRED;
  float distXBLUE = cxBLUE - testxBLUE;
  float distYBLUE = cyBLUE - testyBLUE;
  float distance1 = sqrt( (distXRED * distXRED) + (distYRED * distYRED) );
  float distance2 = sqrt( (distXBLUE * distXBLUE) + (distYBLUE * distYBLUE) );

  if(distance1 <= radii) {
    return true;
  }

  if(distance2 <= radii) {
    return true;
  }

  return false;
}

void update_enemy2(struct Game *game) {
  game->enemy2_rect.x += game->enemy2_xvol;
  game->enemy2_rect.y += game->enemy2_yvol;

  if(game->enemy2_rect.x + game->enemy2_rect.w > WINDOW_WIDTH) {
      game->enemy2_xvol = -5;
  }

  if(game->enemy2_rect.x < 0) {
      game->enemy2_xvol = 5;
  }

  if(game->enemy2_rect.y + game->enemy2_rect.h > WINDOW_HEIGHT) {
      game->enemy2_yvol = -5;
  }

  if(game->enemy2_rect.y < 0) {
      game->enemy2_yvol = 5;
  }

}

void update_enemy1(struct Game *game) {
  game->enemy_rect.x += game->text_xvol;
  game->enemy_rect.y += game->text_yvol;

  if(game->enemy_rect.x + game->enemy_rect.w > WINDOW_WIDTH) {
      game->text_xvol = -3;
  }
  
  if(game->enemy_rect.x < 0) {
      game->text_xvol = 3;
  }

  if(game->enemy_rect.y + game->enemy_rect.h > WINDOW_HEIGHT) {
      game->text_yvol = -3;
  }

  if(game->enemy_rect.y < 0) {
      game->text_yvol = 3;
  }
}

void update_sprite(struct Game *game) {
  if(game->keystate[SDL_SCANCODE_A] || game->keystate[SDL_SCANCODE_LEFT]) {
      game->sprite_rect.x -= game->sprite_vol;
        if(game->keystate[SDL_SCANCODE_LSHIFT]) {
            game->sprite_rect.x = game->sprite_rect.x - 5;
    }
  }
  
  if(game->keystate[SDL_SCANCODE_D] || game->keystate[SDL_SCANCODE_RIGHT]) {
      game->sprite_rect.x += game->sprite_vol;
        if(game->keystate[SDL_SCANCODE_LSHIFT]) {
            game->sprite_rect.x = game->sprite_rect.x + 5;
    }
  }
  
  if(game->keystate[SDL_SCANCODE_W] || game->keystate[SDL_SCANCODE_UP]) {
      game->sprite_rect.y -= game->sprite_vol;
        if(game->keystate[SDL_SCANCODE_LSHIFT]) {
            game->sprite_rect.y = game->sprite_rect.y - 5;
    }
  }

  if(game->keystate[SDL_SCANCODE_S] || game->keystate[SDL_SCANCODE_DOWN]) {
      game->sprite_rect.y += game->sprite_vol;
        if(game->keystate[SDL_SCANCODE_LSHIFT]) {
            game->sprite_rect.y = game->sprite_rect.y + 5;
    }
  }

}

bool menu_nav(struct Menu *menu, SDL_Event event, int a) {
  int x, y;
  int sprite_x = menu->menu_dist.x + menu->menu_dist.w; 
  int sprite_y = menu->menu_dist.y + menu->menu_dist.h;


  SDL_GetMouseState(&x, &y);
  printf("%d\n%d\n", x, y);
  printf("%d\n%d\n", menu->menu_button[0].w, menu->menu_button[0].h);
  if(x >= menu->menu_dist.x && y >= menu->menu_dist.y && x <= sprite_x && y <= sprite_y) {
      printf("button pressed!");
      return true;
   }
    return false;
  }

bool button_highlight(struct Menu *menu, SDL_Event event) {
  int x, y;
  int sprite_x = menu->menu_dist.x + menu->menu_dist.w;
  int sprite_y = menu->menu_dist.y + menu->menu_dist.h;

  SDL_GetMouseState(&x, &y);
  printf("live(x): %d\n live(y): %d\n", x, y);
  if(x >= menu->menu_dist.x && y >= menu->menu_dist.y && x <= sprite_x && y <= sprite_y) {
    printf("hovering\n");
    return true;
  }
  return false;
}
