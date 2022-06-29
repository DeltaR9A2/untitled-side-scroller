#include <stdlib.h>

#include "player.h"

const uint32_t DIR_X = 0;
const uint32_t DIR_R = 1;
const uint32_t DIR_L = 2;

struct player_t{
	body_t *body;
	sprite_t *sprite;
	uint32_t flashing;

	///// Physics Vars
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
	/////
	
	///// Animations
	anim_t *idle_r, *move_r, *skid_r, *jump_r, *hang_r, *fall_r;
	anim_t *idle_l, *move_l, *skid_l, *jump_l, *hang_l, *fall_l;
};

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
	
	player->body = body_create();
	player->sprite = sprite_create();
	
	player->flashing = 0;
   
	player->body->rect->x = 0;
	player->body->rect->y = 0;
	player->body->rect->w = 14;
	player->body->rect->h = 28;
	
	player->fall_speed = 4.00;
	player->fall_accel = 0.10;
	
	player->ground_speed = 2.50;
	player->ground_accel = 0.04;
	player->ground_decel = 0.06;
	
	player->jump_force = -4.0;
	player->jump_brake = -0.5;

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
	sprite_delete(player->sprite);
	body_delete(player->body);
	free(player);
}

void player_update(player_t *player, game_t *game){
	sprite_update(player->sprite);

	if(player->flashing > 0){
		player->flashing -= 1;
	}
	
	player_update_controls(player, game);
	player_update_animation(player, game);
	do_physics_to_it(player->body, game->active_map->terrain_rects, game->active_map->platform_rects);
	sprite_move_to(player->sprite, player->body->rect);
}

void player_update_controls(player_t *player, game_t *game){
	player->body->flags &= ~PLAT_DROP;

	if(player->body->vy < player->fall_speed){
		player->body->vy = fmin(player->fall_speed, player->body->vy + player->fall_accel);
	}

	if(controller_pressed(game->controller, BTN_D)){
		player->body->flags |= PLAT_DROP;
	}
	
	if(controller_just_pressed(game->controller, BTN_U)){
		if(player->body->flags & BLOCKED_D && controller_released(game->controller, BTN_D)){
			player->body->vy = player->jump_force;
		}
	}else if(controller_just_released(game->controller, BTN_U)){
		if(player->body->vy < player->jump_brake){
			player->body->vy = player->jump_brake;
		}
	}
	
	if(controller_pressed(game->controller, BTN_R)){
		player->face_dir = DIR_R;
		player->ctrl_dir = DIR_R;
		if(player->body->vx < 0){
			player->body->vx += player->ground_decel;
		}else if(player->body->vx < player->ground_speed){
			player->body->vx = fmin(player->ground_speed, player->body->vx + player->ground_accel);
		}
	}else if(controller_pressed(game->controller, BTN_L)){
		player->face_dir = DIR_L;
		player->ctrl_dir = DIR_L;
		if(player->body->vx > 0){
			player->body->vx -= player->ground_decel;
		}else if(player->body->vx > -player->ground_speed){
			player->body->vx = fmax(-player->ground_speed, player->body->vx - player->ground_accel);
		}
	}else{
		player->ctrl_dir = DIR_X;
		if(player->body->vx > 0.0){
			player->body->vx = fmax(0.0, player->body->vx - player->ground_decel);
		}else if(player->body->vx < 0.0){
			player->body->vx = fmin(0.0, player->body->vx + player->ground_decel);
		}
	}
	
	if(player->body->vx > 0 && !(player->body->flags & BLOCKED_R)){
		player->move_dir = DIR_R;
	}else if(player->body->vx < 0 && !(player->body->flags & BLOCKED_L)){
		player->move_dir = DIR_L;
	}else{
		player->move_dir = DIR_X;
	}
}

void player_update_animation(player_t *player, game_t *game){
	if(player->body->flags & BLOCKED_D){
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
			if(player->body->vy < -1.0){
				sprite_anim_set(player->sprite, player->jump_r);
			}else if(player->body->vy > 1.0){
				sprite_anim_set(player->sprite, player->fall_r);
			}else{
				sprite_anim_set(player->sprite, player->hang_r);
			}
		}else if(player->face_dir == DIR_L){
			if(player->body->vy < -1.0){
				sprite_anim_set(player->sprite, player->jump_l);
			}else if(player->body->vy > 1.0){
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
	return player->body->rect;
}

uint32_t player_get_facing(player_t *player){
	return player->face_dir;
}

void player_move_to_coord(player_t *player, double x, double y){
	rect_set_center(player->body->rect, x, y);
}