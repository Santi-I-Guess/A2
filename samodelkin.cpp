
#include <cctype>
#include <cstdio>
#include <cstring>
#include <random>

// __linux__ is POSIX compliant
// unsure about _WIN32 vs 

#include "include/helpers.h"
#include "include/samodelkin.h"

void armor_room(struct game_state *alpha) {
        printf("There may be something in this room...");
        if (alpha->has_armor) {
                printf(" and it seems you already have the armor. Good job\n");
                return;
        }

        printf(" would you like to pick up something? ");
        char response[80];
        fgets(response, sizeof(response), stdin);
        if (strchr(response, '\n'))
                response[strcspn(response, "\n")] = '\0';
        else
                clear_extra_stdin();
        switch (tolower(response[0])) {
        case 'y':
                printf("You found a set of armor! Good job!\n");
                alpha->has_armor = true;
                break;
        case 'n':
                printf("That may be a bad choice... but you can come back...\n");
                break;
        default:
                printf("I don't understand that... but you can come back...\n");
                break;
        }
        alpha->visited[ARMOR_ROOM - 1] = true;
}

void empty_room() {
        printf("There's nothing in here...\n");
}

void enemy_room(struct game_state *alpha) {
        printf("You walk towards a figure in front of you.\n");
        printf("They only have one thing to say to you:\n");
        os_independent_sleep(500000);
        enemy_quip(alpha);
        printf("\n");
        os_independent_sleep(1000000);

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> uid_enemy_health(20, 35);
        std::uniform_int_distribution<> uid_found_gold(1, 25);
        int enemy_health = (int)uid_enemy_health(mt);
        int found_gold = (int)uid_found_gold(mt);

        printf("They seem to have around %d hp. You choose to fight\n", enemy_health);

        int hero_min_attack, hero_max_attack;
        if (alpha->has_weapon) {
                hero_min_attack = 10;
                hero_max_attack = 15;
        } else {
                hero_min_attack = 1;
                hero_max_attack = 5;
        }

        int enemy_min_attack, enemy_max_attack;
        if (alpha->has_armor) {
                enemy_min_attack = 5;
                enemy_max_attack = 10;
        } else {
                enemy_min_attack = 2;
                enemy_max_attack = 5;
        }

        std::uniform_int_distribution<> uid_enemy_dmg(enemy_min_attack, enemy_max_attack);
        std::uniform_int_distribution<> uid_hero_dmg(hero_min_attack, hero_max_attack);

        while (1) {
                int hero_dmg = uid_hero_dmg(mt);
                if (alpha->has_weapon)
                        printf("You slash the shadow, dealing %d dmg\n", hero_dmg);
                else
                        printf("You bash the shadow, dealing %d dmg\n", hero_dmg);
                enemy_health -= hero_dmg;

                os_independent_sleep(500000);

                int enemy_dmg = uid_enemy_dmg(mt);
                printf("The figure attacks you in defense, dealing %d dmg\n", enemy_dmg);
                if (alpha->has_armor) {
                        printf("The room rings as your armor bluntens\n");
                }

                os_independent_sleep(500000);

                alpha->hero_health -= enemy_dmg;

                bool first = alpha->hero_health <= 0;
                bool second = enemy_health <= 0;

                if (first && second) {
                        printf("And at the same moment, you both fall\n");
                        alpha->curr_state = LOST;
                        break;
                } else if (first) {
                        printf("And at that moment, you fall\n");
                        alpha->curr_state = LOST;
                        break;
                } else if (second) {
                        printf("And at that moment, the shadow falls\n");
                        printf("You find %d gold in it's pockets. It's yours now\n", found_gold);
                        alpha->hero_gold += found_gold;
                        return;
                }

        }
}

void enter_room(int room_num, struct game_state *alpha) {
        switch (room_num) {
                case KEY_ROOM:    key_room(alpha);    return;
                case WEAPON_ROOM: weapon_room(alpha); return;
                case ARMOR_ROOM:  armor_room(alpha);  return;
                case EXIT_ROOM:   exit_room(alpha);   return;
                default:          break;
        }

        if (alpha->visited[room_num - 1]) {
                printf("You tried going back in room %d, but it was sealed shut...\n", room_num);
                return;
        }

        printf("You creep your way into room %d...\n", room_num);
        static int num_rooms_store[4] = { 10, 25, 50, 100 };
        static int num_rooms = num_rooms_store[alpha->difficulty - 1];

        if (room_num < (num_rooms * .2)) 
                empty_room();
        else if (room_num < (num_rooms * .4))
                potion_room(alpha);
        else
                enemy_room(alpha);
        alpha->visited[room_num - 1] = true;
}


void exit_room(struct game_state *alpha) {
        printf("Here is a locked gate... ");
        if (alpha->has_key) {
                printf("you take hold of your key...\n");
                alpha->curr_state = WON;
        } else {
                printf("you walk away... but you can come back...\n");
        }
        alpha->visited[EXIT_ROOM - 1] = true;
}

int generate_random_room_number(struct game_state *alpha) {
        int num_rooms_store[4] = { 10, 25, 50, 100 };
        int num_rooms = num_rooms_store[alpha->difficulty - 1];

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> uid(1, num_rooms);

        return (int)uid(mt);
}


void key_room(struct game_state *alpha) {
        printf("There may be something in this room...");
        if (alpha->has_key) {
                printf(" and it seems you already have the key. Good job\n");
                return;
        }

        printf(" would you like to pick up something? ");
        char response[80];
        fgets(response, sizeof(response), stdin);
        if (strchr(response, '\n'))
                response[strcspn(response, "\n")] = '\0';
        else
                clear_extra_stdin();
        switch (tolower(response[0])) {
        case 'y':
                printf("You found a key! Good job!\n");
                alpha->has_key = true;
                break;
        case 'n':
                printf("That may be a bad choice... but you can come back...\n");
                break;
        default:
                printf("I don't understand that... but you can come back...\n");
                break;
        }
        alpha->visited[KEY_ROOM - 1] = true;
}

void potion_room(struct game_state *alpha) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> uid_heal(5, 15);
        int healed = (int)uid_heal(mt);

        os_independent_sleep(500000);
        peek_sign(alpha);
        os_independent_sleep(1000000);
        printf("There's a potion of healing on the floor. Restored %d hp\n", healed);
        alpha->hero_health += healed;
}

void weapon_room(struct game_state *alpha) {
        printf("There may be something in this room...");
        if (alpha->has_weapon) {
                printf(" and it seems you already have the weapon. Good job\n");
                return;
        }

        printf(" would you like to pick up something? ");
        char response[80];
        fgets(response, sizeof(response), stdin);
        if (strchr(response, '\n'))
                response[strcspn(response, "\n")] = '\0';
        else
                clear_extra_stdin();
        switch (tolower(response[0])) {
        case 'y':
                printf("You found a weapon! Good job!\n");
                alpha->has_weapon = true;
                break;
        case 'n':
                printf("That may be a bad choice... but you can come back...\n");
                break;
        default:
                printf("I don't understand that... but you can come back...\n");
                break;
        }
        alpha->visited[WEAPON_ROOM - 1] = true;
}

