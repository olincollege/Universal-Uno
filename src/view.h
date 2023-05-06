#pragma once

#include<stdio.h>
#include <stdlib.h>

// The game view struct represented by the message, player id, turn, top card,
// cards in players hand, number of players, and each players number of cards.
typedef struct {
    int message;
    int player_id;
    int turn;
    char* top_card;
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
 * This includes the players hand. As well as the overall game state.
 * The overall game state has the top card and the number of cards that the 
 * other players have. It also prints information for the player on how to play 
 * the game.
 *
 * @param game_v the game view struct
 */
void print_game(game_view* game_v) ;
