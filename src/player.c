#include "player.h"
#include <engine/graphics/renderer.h>
#include <engine/settings.h>
#include <engine/logger.h>


Player *player_create(int accel, int max_speed, int max_hp, int max_mana) {
	Player *p = malloc(sizeof(Player));
	p->accel = accel;
	p->max_hp = max_hp;
	p->max_mana = max_mana;
	p->mana = max_mana;
	p->hp = max_hp;
	p->rect = util_rect(0,0, 32, 32);
	p->speed.x = 0;
	p->speed.y = 0;
	p->max_speed = max_speed;

	Rect screen = util_screen();

	p->hp_bar = pb_create(screen.w / 3, 25, COLOR_WHITE, COLOR_WHITE, COLOR_RED);
	p->mana_bar = pb_create(screen.w / 3, 25, COLOR_WHITE, COLOR_WHITE, COLOR_BLUE);
	p->hp_bar->progress = 100;
	p->mana_bar->progress = 100;

	util_rect_center(&screen, &p->hp_bar->rect);
	util_rect_center(&screen, &p->mana_bar->rect);

	p->mana_bar->rect.y = screen.h - p->mana_bar->rect.h - 5;
	p->hp_bar->rect.y = p->mana_bar->rect.y - p->hp_bar->rect.h - 5;

	return p;
}

void player_free(Player *p) {
	pb_free(p->hp_bar);
	pb_free(p->mana_bar);
	free(p);
}

void player_move(Player *p, Point m) {
	p->rect.x += m.x;
	p->rect.y += m.y;
}

void player_render(Player *p) {
	render_use_camera(1);
	render_color_s(COLOR_GOLD);
	render_rect_s(&p->rect, 1);

	render_use_camera(0);
	render_pb(p->hp_bar);
	render_pb(p->mana_bar);
}

int player_collide(Player *p, Point speed, Tilemap *t, Rect *rects, Rect *intersection) {
	Rect a = p->rect;
	a.x += speed.x;
	a.y += speed.y;
	for(int y = a.y; y < a.y + a.h + t->tileSize / 2; y+=t->tileSize / 2) {
		for(int x = a.x; x < a.x + a.w + t->tileSize / 2; x+=t->tileSize / 2) {
			Point tileCoord = util_world_to_tilemap(t, util_point(x, y));
			Tile *tile = tilemap_get(t, tileCoord.x, tileCoord.y);

			if(tile->type == TILE_WALL) {
				Rect rect = tilemap_get_tile_rect(t, tileCoord.x, tileCoord.y);
				if(util_rect_intersects(&rect, &a, intersection)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

void player_update(Player *p, Tilemap *t, Rect *rects) {
	double delta = util_delta_time();
	double deltaS = delta / 1000;
	float friction = 1 - 0.05f;
	float directionX = 0;
	float directionY = 0;

	if(util_is_keypress(SDL_SCANCODE_D))
		directionX = 1;
	else if(util_is_keypress(SDL_SCANCODE_A))
		directionX = -1;
	if(util_is_keypress(SDL_SCANCODE_W))
		directionY = -1;
	else if(util_is_keypress(SDL_SCANCODE_S))
		directionY = 1;

	float length = sqrt(pow(directionX, 2) + pow(directionY, 2));

	p->speed.x += length != 0 ? p->accel * (directionX / length) : 0;
	p->speed.y += length != 0 ? p->accel * (directionY / length) : 0;


	if(p->speed.x > p->max_speed)
		p->speed.x = p->max_speed;
	else if(p->speed.x < -p->max_speed)
		p->speed.x = -p->max_speed;
	if(p->speed.y > p->max_speed)
		p->speed.y = p->max_speed;
	else if(p->speed.y < -p->max_speed)
		p->speed.y = -p->max_speed;

	Point speed;
	speed.x = p->speed.x * deltaS;
	speed.y = p->speed.y * deltaS;
	Rect col;

	if(player_collide(p, util_point(speed.x, 0), t, NULL, &col)) {
		Rect *r = &p->rect;
		int x = p->rect.x + speed.x;
		int y = p->rect.y + speed.y;

		int left = col.x;
		int right = col.x + col.w;

		if(x <= right && x + r->w >= right && speed.x < 0) {
			log_info("col object on the left\n");
			speed.x = 0;
		}
		else if(x + r->w >= left && x <= left && speed.x > 0) {
			log_info("col object on the right\n");
			speed.x = 0;
		}
	}

	if(player_collide(p, util_point(0, speed.y), t, NULL, &col)) {
		Rect *r = &p->rect;
		int y = p->rect.y + speed.y;

		int top = col.y;
		int bottom = col.y + col.h;

		if(y <= bottom && y + r->h >= bottom && speed.y < 0) {
			log_info("col object up\n");
			speed.y = 0;
		}
		else if(y + r->h >= top && y <= top && speed.y > 0) {
			log_info("col object down\n");
			speed.y = 0;
		}
	}
	player_move(p, speed);
	p->speed.x = p->speed.x * friction;
	p->speed.y = p->speed.y * friction;
}
