#include <stdlib.h>

#include "player.h"
#include "map.h"

#include "stb_ds.h"

const uint32_t DIR_X = 0;
const uint32_t DIR_R = 1;
const uint32_t DIR_L = 2;

const uint32_t BLOCKED_L =    0x00000001;
const uint32_t BLOCKED_R =    0x00000002;
const uint32_t BLOCKED_U =    0x00000004;
const uint32_t BLOCKED_D =    0x00000008;
const uint32_t BLOCKED_MASK = 0x0000000F;

const uint32_t HIT_MAP_EDGE_L =    0x00000010;
const uint32_t HIT_MAP_EDGE_R =    0x00000020;
const uint32_t HIT_MAP_EDGE_U =    0x00000040;
const uint32_t HIT_MAP_EDGE_D =    0x00000080;
const uint32_t HIT_MAP_EDGE_MASK = 0x000000F0;

const uint32_t PLAT_DROP = 0x00000100;

struct player_t{
	sprite_t *sprite;

  rect_t *rect;
  double vx, vy;
  uint32_t flags;

	double fall_speed;
	double fall_accel;
	
	double ground_speed;
	double ground_accel;
	double ground_decel;
	
	double jump_force;
	double jump_brake;
	
	uint32_t face_dir;
	uint32_t ctrl_dir;
	uint32_t move_dir;

	anim_t *idle_r, *move_r, *skid_r, *jump_r, *hang_r, *fall_r;
	anim_t *idle_l, *move_l, *skid_l, *jump_l, *hang_l, *fall_l;
};

static player_t *THE_PLAYER = NULL;

static void load_player_animations(void){
	static bool animations_are_loaded;
	
	if(animations_are_loaded == false){
		animations_are_loaded = true;
        
		fset_t *player_frames = fset_load("player_warrior.png", 8, 8);
		
		anim_create("player_idle_r", player_frames,  0, 6, 10);
		anim_create("player_move_r", player_frames,  8, 8, 10);
		anim_create("player_skid_r", player_frames,  6, 1, 10);

		anim_create("player_jump_r", player_frames, 16, 3, 10);
		anim_create("player_hang_r", player_frames, 19, 2, 10);
		anim_create("player_fall_r", player_frames, 21, 3, 10);

		anim_create("player_idle_l", player_frames, 32, 6, 10);
		anim_create("player_move_l", player_frames, 40, 8, 10);
		anim_create("player_skid_l", player_frames, 38, 1, 10);

		anim_create("player_jump_l", player_frames, 48, 3, 10);
		anim_create("player_hang_l", player_frames, 51, 2, 10);
		anim_create("player_fall_l", player_frames, 53, 3, 10);
	}
}

player_t *player_create(void){
	player_t *player = malloc(sizeof(player_t));
	
	player->sprite = sprite_create();
  player->rect = rect_create();

  player->rect->x = 0;
  player->rect->y = 0;
  player->rect->w = 20;
  player->rect->h = 30;

  player->vx = 0;
  player->vy = 0;

	player->fall_speed = 5.00;
	player->fall_accel = 0.12;
	
	player->ground_speed = 2.25;
	player->ground_accel = 0.06;
	player->ground_decel = 0.12;
	
	player->jump_force = -4.0;
	player->jump_brake = -0.4;

	player->face_dir = DIR_R;
	player->ctrl_dir = DIR_X;
	player->move_dir = DIR_X;
	
	load_player_animations();
	player->idle_r = anim_get("player_idle_r");
	player->idle_l = anim_get("player_idle_l");
	player->move_r = anim_get("player_move_r");
	player->move_l = anim_get("player_move_l");
	player->skid_r = anim_get("player_skid_r");
	player->skid_l = anim_get("player_skid_l");
	player->jump_r = anim_get("player_jump_r");
	player->jump_l = anim_get("player_jump_l");
	player->hang_r = anim_get("player_hang_r");
	player->hang_l = anim_get("player_hang_l");
	player->fall_r = anim_get("player_fall_r");
	player->fall_l = anim_get("player_fall_l");

	return player;
}

void player_delete(player_t *player){
    rect_delete(player->rect);
	sprite_delete(player->sprite);
	free(player);
}

void player_update_controls(player_t *player, controller_t *controller){
	player->flags &= ~PLAT_DROP;

	if(player->vy < player->fall_speed){
		player->vy = fmin(player->fall_speed, player->vy + player->fall_accel);
	}

	if(controller_pressed(BTN_D)){
		player->flags |= PLAT_DROP;
	}
	
	if(controller_just_pressed(BTN_U)){
		if(player->flags & BLOCKED_D && controller_released(BTN_D)){
			player->vy = player->jump_force;
		}
	}else if(controller_just_released(BTN_U)){
		if(player->vy < player->jump_brake){
			player->vy = player->jump_brake;
		}
	}
	
	if(controller_pressed(BTN_R)){
		player->face_dir = DIR_R;
		player->ctrl_dir = DIR_R;
		if(player->vx < 0){
			player->vx += player->ground_decel;
		}else if(player->vx < player->ground_speed){
			player->vx = fmin(player->ground_speed, player->vx + player->ground_accel);
		}
	}else if(controller_pressed(BTN_L)){
		player->face_dir = DIR_L;
		player->ctrl_dir = DIR_L;
		if(player->vx > 0){
			player->vx -= player->ground_decel;
		}else if(player->vx > -player->ground_speed){
			player->vx = fmax(-player->ground_speed, player->vx - player->ground_accel);
		}
	}else{
		player->ctrl_dir = DIR_X;
		if(player->vx > 0.0){
			player->vx = fmax(0.0, player->vx - player->ground_decel);
		}else if(player->vx < 0.0){
			player->vx = fmin(0.0, player->vx + player->ground_decel);
		}
	}
	
	if(player->vx > 0 && !(player->flags & BLOCKED_R)){
		player->move_dir = DIR_R;
	}else if(player->vx < 0 && !(player->flags & BLOCKED_L)){
		player->move_dir = DIR_L;
	}else{
		player->move_dir = DIR_X;
	}
}

void player_select_animation(player_t *player){
	if(player->flags & BLOCKED_D){
		if(player->move_dir == DIR_X){
			if(player->face_dir == DIR_R){
				sprite_anim_set(player->sprite, player->idle_r);
			}else if(player->face_dir == DIR_L){
				sprite_anim_set(player->sprite, player->idle_l);
			}
		}else if(player->move_dir == DIR_R){
			if(player->ctrl_dir == DIR_R){
				sprite_anim_set(player->sprite, player->move_r);
			}else{
				sprite_anim_set(player->sprite, player->skid_r);
			}
		}else if(player->move_dir == DIR_L){
			if(player->ctrl_dir == DIR_L){
				sprite_anim_set(player->sprite, player->move_l);
			}else{
				sprite_anim_set(player->sprite, player->skid_l);
			}
		}
	}else{
		if(player->face_dir == DIR_R){
			if(player->vy < -1.0){
				sprite_anim_set(player->sprite, player->jump_r);
			}else if(player->vy > 1.0){
				sprite_anim_set(player->sprite, player->fall_r);
			}else{
				sprite_anim_set(player->sprite, player->hang_r);
			}
		}else if(player->face_dir == DIR_L){
			if(player->vy < -1.0){
				sprite_anim_set(player->sprite, player->jump_l);
			}else if(player->vy > 1.0){
				sprite_anim_set(player->sprite, player->fall_l);
			}else{
				sprite_anim_set(player->sprite, player->hang_l);
			}
		}
	}
    
}

sprite_t *player_get_sprite(player_t *player){
	return player->sprite;
}

rect_t *player_get_rect(player_t *player){
	return player->rect;
}

uint32_t player_get_facing(player_t *player){
	return player->face_dir;
}

void player_move_to_rect(player_t *player, rect_t *rect){
	rect_move_to(player->rect, rect);
}

player_t *player_get_only(void){
    if(THE_PLAYER == NULL){
        THE_PLAYER = player_create();
    }
    return THE_PLAYER;
}

void player_cleanup(void){
    if(THE_PLAYER != NULL){
        player_delete(THE_PLAYER);
    }
}

void player_move_and_collide_with_map(player_t *player, map_t *map){
  rect_t *map_bounds = map->rect;
  rect_t *terr_rects = map->terrain_rects;
  rect_t *plat_rects = map->platform_rects;
    
	rect_t start = (rect_t){0,0,0,0};
	rect_match_to(&start, player->rect);
	
	player->flags &= ~BLOCKED_MASK;
	
	player->rect->x += player->vx;
	for(int i=0; i<arrlen(terr_rects); i++){
        rect_t *curr_rect = &terr_rects[i];
		if(rect_overlap(player->rect, curr_rect)){
			if(player->vx > 0 && rect_get_r_edge(player->rect) >= rect_get_l_edge(curr_rect)){
				player->vx = 0;
				rect_set_r_edge(player->rect, rect_get_l_edge(curr_rect));
				player->flags |= BLOCKED_R;
			}else if(player->vx < 0 && rect_get_l_edge(player->rect) <= rect_get_r_edge(curr_rect)){
				player->vx = 0;
				rect_set_l_edge(player->rect, rect_get_r_edge(curr_rect));
				player->flags |= BLOCKED_L;
			}
			break;
		}
	}

	player->rect->y += player->vy;
	for(int i=0; i<arrlen(terr_rects); i++){
        rect_t *curr_rect = &terr_rects[i];
		if(rect_overlap(player->rect, curr_rect)){
			if(player->vy > 0 && rect_get_b_edge(player->rect) >= rect_get_t_edge(curr_rect)){
				player->vy = 0;
				rect_set_b_edge(player->rect, rect_get_t_edge(curr_rect));
				player->flags |= BLOCKED_D;
			}else if(player->vy < 0 && rect_get_t_edge(player->rect) <= rect_get_b_edge(curr_rect)){
				player->vy = 0;
				rect_set_t_edge(player->rect, rect_get_b_edge(curr_rect));
				player->flags |= BLOCKED_U;
			}
			break;
		}
	}
	
	if(player->vy > 0 && !(player->flags & PLAT_DROP)){
    for(int i=0; i<arrlen(plat_rects); i++){
      rect_t *curr_rect = &plat_rects[i];
			if(rect_overlap(player->rect, curr_rect)){
				if(rect_get_b_edge(&start) <= rect_get_t_edge(curr_rect)){
					player->vy = 0;
					rect_set_b_edge(player->rect, rect_get_t_edge(curr_rect));
					player->flags |= BLOCKED_D;
					break;
				}
			}
		}
	}
    
    player->flags &= ~HIT_MAP_EDGE_MASK;
    if(!rect_inside_of(player->rect, map_bounds)){
        if(rect_get_r_edge(player->rect) > rect_get_r_edge(map_bounds)){
          player->flags |= HIT_MAP_EDGE_R;
        }else if(rect_get_l_edge(player->rect) < rect_get_l_edge(map_bounds)){
          player->flags |= HIT_MAP_EDGE_L;
        }
        
        if(rect_get_b_edge(player->rect) > rect_get_b_edge(map_bounds)){
          player->flags |= HIT_MAP_EDGE_D;
        }else if(rect_get_t_edge(player->rect) < rect_get_t_edge(map_bounds)){
          player->flags |= HIT_MAP_EDGE_U;
        }
        
        rect_limit_to(player->rect, map_bounds);
    }
}

// Player is placed this distance from the edge when entering a map.
static const double MAP_ENTRY_OFFSET = 2;
void player_check_for_map_transition(player_t *player, game_t *game){
  if(player->flags & HIT_MAP_EDGE_MASK){
    int32_t starting_world_x = game->map_world_x;
    int32_t starting_world_y = game->map_world_y;
    
    int32_t player_offset_x = (int32_t)(rect_get_mid_x(player->rect)/WORLD_CELL_W);
    int32_t player_offset_y = (int32_t)(rect_get_mid_y(player->rect)/WORLD_CELL_H);

    int32_t direction_adjust_x = 0;
    int32_t direction_adjust_y = 0;
  
    if(player->flags & HIT_MAP_EDGE_R){
      direction_adjust_x += 1; //map_get_cell_w(game->active_map);
    }else if(player->flags & HIT_MAP_EDGE_L){
      direction_adjust_x -= 1;
    }else if(player->flags & HIT_MAP_EDGE_D){
      direction_adjust_y += 1; //map_get_cell_h(game->active_map);
    }else if(player->flags & HIT_MAP_EDGE_U){
      direction_adjust_y -= 1;
    }

    int32_t target_world_x = starting_world_x + player_offset_x + direction_adjust_x;
    int32_t target_world_y = starting_world_y + player_offset_y + direction_adjust_y;

    if(map_exists_at_coords(target_world_x, target_world_y)){
      game_select_map(game, target_world_x, target_world_y);
          
      map_t *new_map = game->active_map;
          
      int32_t relative_world_x = target_world_x - new_map->world_x;
      int32_t relative_world_y = target_world_y - new_map->world_y;
          
      int32_t player_x_pos_correction = (relative_world_x - player_offset_x) * WORLD_CELL_W;
      int32_t player_y_pos_correction = (relative_world_y - player_offset_y) * WORLD_CELL_H;
          
      if(player->flags & HIT_MAP_EDGE_R){
        rect_set_l_edge(player->rect, MAP_ENTRY_OFFSET);
        player->rect->y += player_y_pos_correction;
        //player->vy = 0;
      }else if(player->flags & HIT_MAP_EDGE_L){
        rect_set_r_edge(player->rect, map_get_w(game->active_map) - MAP_ENTRY_OFFSET);
        player->rect->y += player_y_pos_correction;
        //player->vy = 0;
      }else if(player->flags & HIT_MAP_EDGE_D){
        rect_set_t_edge(player->rect, MAP_ENTRY_OFFSET);
        player->rect->x += player_x_pos_correction;
      }else if(player->flags & HIT_MAP_EDGE_U){
        rect_set_b_edge(player->rect, map_get_h(game->active_map) - MAP_ENTRY_OFFSET);
        player->rect->x += player_x_pos_correction;
      }            
    }
  }
}

void player_update(player_t *player, game_t *game){
	player_update_controls(player, game->controller);
  player_move_and_collide_with_map(player, game->active_map);
  player_check_for_map_transition(player, game);
	player_select_animation(player);
  sprite_move_to(player->sprite, player->rect);
	sprite_anim_update(player->sprite);
}
