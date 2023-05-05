#pragma once

#include<stdio.h>
#include <stdlib.h>


typedef struct {
    int message;
    int player_id;
    int turn;
    char *top_card;
    int cards_in_hand;
    char *hand;
    int number_players;
    int player_0;
    int player_1;
    int player_2;
    int player_3;
    int player_4;
} game_view;

/**
 * Print a text representation of the uno game.
 *
 * @param game_v the game view struct
 */
void print_game(game_view* game_v) ;
