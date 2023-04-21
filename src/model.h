#pragma once

#include<stdio.h>

// Define useful constants needed in the program.
enum {
  /** Number of cards for UNO.*/
  UNO = 1,
  /** Maximum numbers of players*/
  PLAYERS = 5,
  /** Number of cards in a deck without blank cards.*/
  DECK= 108,

};

// A represented color of the card and value.
//Special moves are higher numbers 
// r, b g, y, a
// 10 = reverse turn 
// 11 = skip 
// 12 = draw 2 
// 13 = draw 4
// 14 = pick color 
// 15 = pick color + draw 4
typedef struct {
  char color;
  size_t value;
  card* next; 
} card;

// deck of cards represented by a linked list
typedef struct {
  card* head;  
  size_t size;
} deck;

//Struct representing a player 
typedef struct {
  size_t number;
  deck hand;
  int sock_num; 
  player* next; 
  player* prev; 
} player;

//Struct representing player turns (possible global)
typedef struct {
  player* head; 
  player* cur; 
  size_t direction; 
} turn;

//Struct representing game struct
typedef struct {
  card main;
  turn current; 
  deck draw; 
  deck discard; 
  player* players[PLAYERS];
  size_t end;
} game_state;


/*
Make a new card 
*/
card* make_card(char color, size_t value, size_t special); 

/*
move card 
*/
int move_card(card* card, deck* old_deck, deck* new_deck);

/*
Delete card 
*/

void delete_card(card* card);

/*
Delete deck();
*/
void delete_deck(deck* card);

/*
Make new player
*/
player* make_player(void);

/*
Delete Player 
*/
int delete_player(player* user);

/*
* Make a new deck.
* @param var A pointer to a new deck 
*/
deck* make_deck(void);

deck* make_UNO_deck(void);

/**
 * Update the UNO board with the player's latest move.
 * 
 * REMOVE ONE CARD/ADD CARD, UPDATE MAIN, MOVE TO DISCARD,
 * 
 * @param var A pointer to the variable game state containing the player board
 * and the player's move.
 */
void update_moves(game_state *var);

/**
 * Shuffle Discard pile, and move into draw pile 
 * @param var A pointer to the variable game state struct.
 * 
 */
void refill_draw(game_state *var);

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
void update_turn(game_state *var);


/**
 * check UNO
 *
 * @param var A pointer to the variable game state struct.
 */
int check_uno(game_state *var);

/**
 * check end
 *
 * @param var A pointer to the variable game state struct.
 */
int check_end(game_state *var);

void update_player_turn(game_state *var);

void update_player_turn(game_state *var);
