#pragma once
#include "model.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Define useful constants needed in the program.
enum {
  /** Reverse Card Value */
  REVERSE = 10,
  /** Skip Card Value*/
  SKIP = 11,
  /** Draw 2 Card Value*/
  DRAW_2 = 12,
  /** Draw 4 WildCard Value*/
  DRAW_4 = 13,
  /** WildCard Value*/
  WILD = 14
};

/**
 * Processes an input from the client and turns the card input into a string
 * The string should start with a character representing the color of the card
 * And end with a number representing the type of card.
 *  *0-9: regular uno cards
 *  *10: Reverse card
 *  *11: Skip card
 *  *12: draw2 card
 *  *13: draw4 wildcard
 *  *14: wildcard
 * 
 * @param buf: a char array with the input from the client
 * @param processed_input: a char array to store the processed input
 */
void process_input(char* buf, char* processed_input);
/**
 * Checks if a card is in a player's hand 
 * 
 * @param played_card: a card object representing the card played
 * @param hand: a deck object representing the player's hand
 * 
 * @return int: returns 1 if the card is in the hand, returns 0 otherwise
 */
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
 * @return int: returns 0 if the card is not valid. If it's valid, return 1
 * 
 */
int is_valid(char* card, game_state* game_state_);
/**
 * Changes the current player depending on the game direction. 
 * If the game direction is 0, move to the next player in the lineup
 * If the game direction is 1, move to the previous player in the lineup
 * 
 * @param game_state_: a pointer to a game_state object holding the current game state
 */
void change_turn(game_state* game_state_);
