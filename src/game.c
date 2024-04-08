#include <stdlib.h>
#include <string.h>

#include <SDL.h>

#include "game.h"
#include "rect.h"

static game_t *THE_GAME;

const uint32_t GAME_MODE_MENU = 0;
const uint32_t GAME_MODE_PLAY = 1;
const uint32_t GAME_MODE_DIALOGUE = 2;

const uint32_t GAME_MESSAGE_LEN = 128;

void game_create_data_structures(game_t *game);
void game_delete_data_structures(game_t *game);

void game_set_message(game_t *game, const char *text){
  game->message_timeout = 240;
  sprintf(game->message, "%s", text);

  SDL_Rect fill_rect;
  fill_rect.x = 0;
  fill_rect.y = 0;
  fill_rect.w = game->message_surface->w;
  fill_rect.h = game->message_surface->h;
  SDL_FillRect(game->message_surface, &fill_rect, 0x666666FF);

  fill_rect.x += 2;
  fill_rect.y += 2;
  fill_rect.w -= 4;
  fill_rect.h -= 4;
  SDL_FillRect(game->message_surface, &fill_rect, 0x333333FF);

  font_draw_string(game->font, game->message, 8, 4, game->message_surface);
}

void game_select_map(game_t *game, const char* map_name){
  map_t *map = map_load_by_name(map_name);

  if(map == NULL){ return; }
    
  game->active_map = map;
  camera_limit_to(game->camera, game->active_map->rect);
}

game_t *game_create(void){
  game_t *game = malloc(sizeof(game_t));

  game->core = core_get_only();
  game->step = 0;
  game->mode = GAME_MODE_PLAY;

  game_create_data_structures(game);

  game->player = player_get_only();

  game->font = font_create("font_nokia.png");
  game->debug_font = font_create("font_nokia.png");

  game_select_map(game, "default");
  
  game->menu = menu_create_main_menu(game);
  
  camera_init(game->camera, VIRTUAL_SCREEN_W, VIRTUAL_SCREEN_H);

  game->message = calloc(GAME_MESSAGE_LEN, sizeof(char));
  game->message_surface = create_surface(640-16, 6+font_get_height(game->font));
  game->message_timeout = 0;

  player_move_to_rect(game->player, &((rect_t){64,64,0,0}));
  sprite_anim_set(player_get_sprite(game->player), anim_get("player_idle_r"));
  player_update(game->player, game);

  return game;
}

void game_delete(game_t *game){
  free(game->message);

  menu_delete(game->menu);  
  font_delete(game->font);

  game_delete_data_structures(game);

  free(game);
}


void game_fast_frame(game_t *game){
  game->step += 1;

  if(game->mode == GAME_MODE_MENU){
    if(controller_just_pressed(BTN_U)){ menu_up(game->menu); }
    if(controller_just_pressed(BTN_D)){ menu_down(game->menu); }
    if(controller_just_pressed(BTN_A)){ menu_activate(game->menu); }
    if(controller_just_pressed(BTN_START)){ menu_activate(game->menu); }
  }else if(game->mode == GAME_MODE_PLAY){
    if(controller_just_pressed(BTN_START)){
      game->mode = GAME_MODE_MENU;
      camera_set_fade(game->camera, 0x000000AA);
      return;
    }

    player_update(game->player, game);
  }
}

void game_full_frame(game_t *game){
  SDL_Rect draw_rect;

  game_fast_frame(game);

  camera_look_at(game->camera, player_get_rect(game->player));
  camera_draw_game(game->camera, game);
  SDL_BlitSurface(camera_get_surface(game->camera), NULL, core_get_screen_surface(game->core), NULL);

  if(game->mode == GAME_MODE_MENU){
    menu_draw(game->menu, core_get_screen_surface(game->core));
  }

  if(game->message_timeout > 0){
    game->message_timeout -= 1;
    draw_rect.x = 8;
    draw_rect.y = VIRTUAL_SCREEN_H - (8+game->message_surface->h);

    SDL_BlitSurface(game->message_surface, NULL, core_get_screen_surface(game->core), &draw_rect);
  }
}

void game_create_data_structures(game_t *game){
  game->camera = camera_create();
}

void game_delete_data_structures(game_t *game){
  camera_delete(game->camera);
}

game_t *game_get_only(void){
  if(THE_GAME == NULL){
    THE_GAME = game_create();
  }
  return THE_GAME;
}

void game_cleanup(void){
  if(THE_GAME != NULL){
    game_delete(THE_GAME);
  }
}
