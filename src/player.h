#ifndef PLAYER_H
#define PLAYER_H

#include <engine/util.h>
#include <engine/ui/progress_bar.h>

typedef struct Player {
	Rect rect;
	Point speed;
	int accel;
	int max_hp;
	int hp;
	int mana;
	int max_mana;
	int max_speed;
	ProgressBar *hp_bar;
	ProgressBar *mana_bar;
} Player;

Player *player_create(int accel, int max_speed, int max_hp, int max_mana);
void player_free(Player *p);

void player_render(Player *p);

void player_move(Player *p, Point m);

#endif
