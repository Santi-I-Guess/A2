
#ifndef HELPERS
#define HELPERS 1

const unsigned int KEY_ROOM    = 3;
const unsigned int WEAPON_ROOM = 5;
const unsigned int ARMOR_ROOM  = 7;
const unsigned int EXIT_ROOM   = 9;

enum playing_state {
        PLAYING,
        WON,
        LOST,
        QUIT
};

struct game_state {
        unsigned int difficulty;     // 1-4
        unsigned int explored_rooms; // 0
        unsigned int curr_state;     // playing
        int hero_health;    // 100
        unsigned int hero_gold;      // 0
        bool has_key;       // false
        bool has_weapon;    // false
        bool has_armor;     // false
        bool visited[100];  // {0}
        bool heard_sign[12];
        bool heard_enemy[12];
};

void clear_extra_stdin();
void enemy_quip(struct game_state *alpha);
void get_difficulty(struct game_state *alpha);
void handle_backtrack(struct game_state *alpha);
void peek_sign(struct game_state *alpha);
void os_independent_sleep(unsigned int milliseconds);

#endif
