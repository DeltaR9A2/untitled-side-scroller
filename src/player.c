#include <stdlib.h>

#include "player.h"
#include "map.h"

#include "stb_ds.h"

const uint32_t DIR_X = 0;
const uint32_t DIR_R = 1;
const uint32_t DIR_L = 2;

const uint32_t BLOCKED_L = 0x00000001;
const uint32_t BLOCKED_R = 0x00000002;
const uint32_t BLOCKED_U = 0x00000004;
const uint32_t BLOCKED_D = 0x00000008;
const uint32_t BLOCKED_MASK = 0x0000000F;

const uint32_t PLAT_DROP = 0x00000010;

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
	
	player->fall_speed = 6.00;
	player->fall_accel = 0.12;
	
	player->ground_speed = 2.50;
	player->ground_accel = 0.08;
	player->ground_decel = 0.10;
	
	player->jump_force = -4.4;
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
    rect_t *terr_rects = map_get_terrain_rects(map);
    rect_t *plat_rects = map_get_platform_rects(map);
    
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
}

void player_update(player_t *player, game_t *game){
	player_update_controls(player, game->controller);
    player_move_and_collide_with_map(player, game->active_map);
	player_select_animation(player);

    sprite_move_to(player->sprite, player->rect);
	sprite_anim_update(player->sprite);
}
