
#ifndef SAMODELKIN
#define SAMODELKIN 1

#include "helpers.h"

void armor_room(struct game_state *alpha);
void empty_room();
void enemy_room(struct game_state *alpha);
void enter_room(int room_num, struct game_state *alpha);
void exit_room(struct game_state *alpha);
int generate_random_room_number(struct game_state *alpha);
void key_room(struct game_state *alpha);
void potion_room(struct game_state *alpha);
void weapon_room(struct game_state *alpha);

#endif
