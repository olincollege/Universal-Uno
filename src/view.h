#pragma once

#include <stdlib.h>
//#include "model.h"

/**
 * Print a text representation of the uno hand.
 *
 * @param game_state the state of each hand
 * @param player the hand
 */
void print_game_state(const game_state* game_state);

/**
 * Print a text representation of the uno hand.
 *
 * @param game_state the state of each hand
 * @param player the hand
 */
void print_player_hand(const game_state* game_state, int player);
