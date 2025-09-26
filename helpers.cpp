#include <cstring>
#include <cstdio>
#include <random>
#include <unordered_map>

#include "include/helpers.h"
#include "include/samodelkin.h"

const char fortune_signs[12][128] = {
        "A body on vacation tends to remain on vacation unless acted upon by an outside force",
        "Life is like an analogy",
        "Chaos is King and Magic is loose in the world",
        "Somewhere, just out of sight, the unicorns are gathering",
        // seperator
        "Using words to describe magic is like using a screwdriver to cut roast beef",
        "Do what thou wilt shall be the whole of the Law",
        "After all, all he did was string together a lot of old, well-known quotations",
        "Go not to the elves for counsel, for they will say both yes and no",
        // seperator
        "Not because it gave pain to the monster, but because it gave pleasure to the spectators",
        "He that teaches himself has a fool for a master",
        "My, how you've changed since I've changed",
        "The brain is a wonderful organ; it does not stop until you get to school",

};

const char fortune_enemy[12][128] = {
        "What on earth would a man do with himself if something did not stand in his way?",
        "There's nothing very mysterious about you, except that nobody really knows your origin, purpose, or destination",
        "Right now I'm having amnesia and deja vu at the same time",
        "Thinking is dangerous.  It leads to ideas",
        // seperator
        "Please forgive me if, in the heat of battle, I sometimes forget which side I'm on",
        "Most people eat as though they were fattening themselves for market",
        "What you don't know won't help you much either",
        "Don't let your mind wander -- it's too little to be let out alone",
        // seperator
        "You're either part of the solution or part of the problem",
        "You plan things that you do not even attempt because of your extreme caution",
        "The mind is its own place, and in itself. Can make a Heav'n of Hell, a Hell of Heav'n.",
        "The smallest worm will turn being trodden on"
};

void clear_extra_stdin() {
        while (1) {
                int curr = fgetc(stdin);
                if (curr == '\n' || curr == EOF) {
                        break;
                }
        }
}

void get_difficulty(struct game_state *alpha) {
        printf("Difficulties:\n\t(1) Easy\n\t(2) Medium\n\t(3) Hard\n\t(4) Insane\n");
        while (1) {
                printf("Select difficulty: ");
                char buffer[80];
                fgets(buffer, sizeof(buffer), stdin);
                // avoid weird buffer overflow & fgets carryover
                if (strchr(buffer, '\n'))
                        buffer[strcspn(buffer, "\n")] = '\0';
                else
                        clear_extra_stdin();
                // logical validation
                int ret = sscanf(buffer, "%5u", &(alpha->difficulty));
                if (ret != 1) {
                        printf("\"%s\" is not a valid int\n", buffer);
                } else if (!(1 <= alpha->difficulty && alpha->difficulty <= 4)) {
                        printf("Keep difficulty within 1-4\n");
                } else {
                        break;
                }
        }
}

void peek_sign(struct game_state *alpha) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> uid_sign(0, 11);
        int sign_num = (int)uid_sign(mt);
        printf("There's a sign inside the room:\n");
        printf("\"%s\"\n", fortune_signs[sign_num]);
        if (alpha->heard_sign[sign_num]) {
                printf("You've read this before...\n");
        } else {
                alpha->heard_sign[sign_num] = true;
        }
}

void enemy_quip(struct game_state *alpha) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> uid_quip(0, 11);
        int quip_num = (int)uid_quip(mt);
        printf("\"%s\"\n", fortune_enemy[quip_num]);
        if (alpha->heard_enemy[quip_num]) {
                printf("You say you've heard that before...\n");
        } else {
                alpha->heard_enemy[quip_num] = true;
        }
}

void handle_backtrack(struct game_state *alpha) {
        bool seen[4] = {
                alpha->visited[KEY_ROOM-1],
                alpha->visited[WEAPON_ROOM-1],
                alpha->visited[ARMOR_ROOM-1],
                alpha->visited[EXIT_ROOM-1]
        };
        if ((int)(seen[0] + (seen[1] << 1) + (seen[2] << 2) + (seen[3] << 3)) == 0) {
                printf("You are but a stranger, and remember no chambers to return to...\n");
                return;
        }

        // only need to initalize once, but doesn't need global scope
        static std::unordered_map<int, const char*> reference = {
                {KEY_ROOM, "Key Room"}, {WEAPON_ROOM, "Weapon Room"},
                {ARMOR_ROOM, "Armor Room"}, {EXIT_ROOM, "Exit Room"}
        };

        printf("You remember the following chambers:\n");

        if      (seen[0] &&  alpha->has_key)    { printf("\n\t[0] %s\n", reference.at(KEY_ROOM)); }
        else if (seen[0] && !alpha->has_key)    { printf("\n\t[0] %s\n", "Mystery Room"); }
        if      (seen[1] &&  alpha->has_weapon) { printf("\n\t[1] %s\n", reference.at(KEY_ROOM)); }
        else if (seen[1] && !alpha->has_weapon) { printf("\n\t[1] %s\n", "Mystery Room"); }
        if      (seen[2] &&  alpha->has_armor)  { printf("\n\t[2] %s\n", reference.at(KEY_ROOM)); }
        else if (seen[2] && !alpha->has_armor)  { printf("\n\t[2] %s\n", "Mystery Room"); }
        if      (seen[3])                      { printf("\n\t[3] %s\n", reference.at(EXIT_ROOM)); }

        char response[80];
        fgets(response, sizeof(response), stdin);
        // avoid weird buffer overflow & fgets carryover
        if (strchr(response, '\n'))
                response[strcspn(response, "\n")] = '\0';
        else
                clear_extra_stdin();
        switch (response[0]) {
                case '0':
                        if (seen[0]) key_room(alpha);
                        else         goto smth;
                        break;
                case '1':
                        if (seen[1]) weapon_room(alpha);
                        else         goto smth;
                        break;
                case '2':
                        if (seen[2]) armor_room(alpha);
                        else         goto smth;
                        break;
                case '3':
                        if (seen[3]) exit_room(alpha);
                        else         goto smth;
                        break;
                // add ability to choose rooms
                default:
                smth:
                        printf("And without a clear image in your mind, "
                               "you fail to backtrack to a room you've seen...\n");
                        break;
        }

        // if the first digit aligns with seen 0-3, go there, else back off
}
