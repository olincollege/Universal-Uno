#pragma once
#include "model.h"


// Define useful constants needed in the program.


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

