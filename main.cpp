
#include <cctype>
#include <cstdio>
#include <cstring>

#include "include/helpers.h"
#include "include/samodelkin.h"

/*

At the start of the game, allow the user to select the game difficulty.
The higher the difficulty, the more rooms that can be randomly generated. For example:

    Easy - 10 rooms
    Medium - 25 rooms
    Hard - 50 rooms
    Insane - 100 rooms

While the game is in a playing state

    Generate a random room number
    Enter the room
    Increment the number of rooms explored
    If after visiting, still in a playing state ask if the user wishes to continue exploring. If no, then set the game state to quit.

Print how many rooms were explored.
If the user won, congratulate the user!
If the user lost, encourage them to play again.
If the user quit, print their inventory and hint they may need to find the various equipment pieces.

Except for the exit room, do not allow a room to be visited more than once.
If the user decides not to pick up the key, weapon, or armor
then they should be allowed to revisit that room. If they pick it up, then they can't return.

*/


int main() {
        struct game_state alpha = { 0, 0, PLAYING, 100, 0, false, false, false, {0}, {0}, {0} };
        get_difficulty(&alpha);

        char asterisk[78] = "";
        for (int i = 0; i < 76; ++i)
                asterisk[i] = '*';

        printf("\n%s\n* You are the hero, an interloper to this dungeon "
               "sent for your own reasons *\n%s\n\n", asterisk, asterisk);
        while (alpha.curr_state == PLAYING) {
                printf("What do you wish to do?:\n"
                       "\t[e] Explore\n"
                       "\t[s] Status check\n"
                       "\t[b] Backtrack\n"
                       "\t[q] Quit\n");
                printf("It's up to you: ");
                char response[80];
                fgets(response, sizeof(response), stdin);
                if (strchr(response, '\n'))
                        response[strcspn(response, "\n")] = '\0';
                else
                        clear_extra_stdin();

                switch (tolower(response[0])) {
                        case 'b':
                                handle_backtrack(&alpha);
                                // printf("you have seen %s", some_rooms);
                                // print("which room will you return to? ");
                                // exit room, key room, armor room, weapon room, IF already found
                                // undefined behaviour results in no room getting passed
                                break;
                        case 'e':
                                enter_room(generate_random_room_number(&alpha), &alpha);
                                break;
                        case 'q':
                                alpha.curr_state = QUIT;
                                break;
                        case 's':
                                printf("You have %dHP, %s key, %s weapon, "
                                       "%s armor, and %d gold to your name\n",
                                       alpha.hero_health,
                                       alpha.has_key    ? "a" : "no",
                                       alpha.has_weapon ? "a" : "no",
                                       alpha.has_armor  ? "a set of" : "no",
                                       alpha.hero_gold
                                       );
                                break;
                        default:
                                printf("I don't know what that means...\n");
                                break;
                }
                // actions: status (inventory), explore, backtrack (to special room, if visited)
                char turn_seperator[33] = "-------------------------------";
                printf("\n%s\n\n", turn_seperator);
        }

        // TODO:
        switch (alpha.curr_state) {
        case WON:
                printf("you won\n");
                break;
        case LOST:
                printf("you lost\n");
                break;
        case QUIT:
                printf("you gave up\n");
                break;
        default:
                printf("You reached an impossible path by some miracle\n");
                return 1;
        }

        printf("\n");

        return 0;
}
