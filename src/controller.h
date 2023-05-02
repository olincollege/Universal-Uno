#pragma once
#include "model.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void process_input(char* buf, char* processed_input);
int in_hand(card* played_card, deck* hand);
/**
 * Checks that a move is valid. By the rules of UNO, a move is only valid if:
 *   * It is the same color as the top card on the played deck, or
 *   * It is the same number as the top card on the played deck, or
 *   * It is a wild card
 * 
 * @param card[]: a string representation of the played card
 * @param game_state_: the current game state
 * 
 * @return int: still determining return values
 * 
 */
int is_valid(char* card, game_state* game_state_);
