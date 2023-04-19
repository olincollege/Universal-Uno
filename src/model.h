#pragma once


// Define useful constants needed in the program.
enum {
  /** Number of errors allowed.*/
  UNO = 1,
  /** Maximum size of input.*/
  PLAYERS = 5,
  /** Number of cards in the deck.*/
  DECK= 112,

};

// Define game state structs.
// The player's card represented color of the card and value.
//Special moves are higher numbers 
typedef struct {
  char color;
  int value;
  char num;
} card;
//POSSIBLE STRUCT FOR PILE OF CARDS OR FOR PLAYER VS VAR
// The parts of the UNO game that changes, including 
//the main card each player status, turn, draw pile, discard pile, and end
typedef struct {
  card main;
  int turn;
  card draw[DECK]; //Maybe need Malloc for dynamic draw
  int draw_size;
  card discard[DECK]; //maybe mallco too
  int players[PLAYERS];
  int end;

} var_game_state;

// The parts of the Uno that do not change, including the initial board,
// the solution board, and the level.
typedef struct {
  card deck[DECK];
  // Possible player info because are only set once at start of game
} const_game_state;

/**
 * Update the UNO board with the player's latest move.
 * 
 * REMOVE ONE CARD/ADD CARD, UPDATE MAIN, MOVE TO DISCARD,
 * 
 * @param var A pointer to the variable game state containing the player board
 * and the player's move.
 */
void update_moves(var_game_state *var);

/**
 * Shuffle Discard pile, and move into draw pile 
 * @param var A pointer to the variable game state struct.
 * 
 */
void refill_draw(var_game_state *var);

/**
 * Shuffle cards
 *
 * @param pile a pile of cards .
 */
void shuffle(card pile[DECK]);

/**
 * Update turn based on move
 * @param var A pointer to the variable game state struct.
 * 
 */
void update_turn(var_game_state *var);


/**
 * check UNO
 *
 * @param var A pointer to the variable game state struct.
 */
int check_uno(var_game_state *var);

/**
 * check end
 *
 * @param var A pointer to the variable game state struct.
 */
int check_end(var_game_state *var);


