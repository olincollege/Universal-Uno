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
    char *hand_sizes;
} game_view;

/**
 * Print a text representation of the uno game.
 *
 * @param top_card the top card of the deck
 * @param hand_size an array of hand sizes in order of player number
 * @param players the length of hand_size which is the number of players
 * @param player_turn an int corresponding to which players turn it is
 */
void print_game(char top_card[], int hand_sizes[], size_t players, int player_turn);

/**
 * Print a text representation of the uno hand and if it's the players turn.
 *
 * @param hand a string that represent the player's hand.
 * @param player_turn an int corresponding to which players turn it is
 * @param player_id an int corresponding to which player this is
 */
void print_hand(char hand[], int player_turn, int player_id);

/**
 * Print an error to let the player know to remove
 */
void print_error();
