#include "player.h"
#include <engine/graphics/renderer.h>
#include <engine/settings.h>


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
