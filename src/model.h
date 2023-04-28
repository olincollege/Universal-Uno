#pragma once

#include<stdio.h>
#include <stdlib.h>

// Define useful constants needed in the program.
enum {
  /** Number of cards for UNO.*/
  UNO = 1,
  /** Maximum numbers of players*/
  MAX_PLAYERS = 5,
  /** Number of cards in a deck*/
  UNO_DECK= 108,
  /** Special Number for Draw 4*/
  SPECIAL_13 = 13, 
  /** Special Number for pick color + draw 4*/
  SPECIAL_14 = 14

};

// A card represented by the color of the card and value.
// Special moves are higher numbers
// 10 = reverse turn
// 11 = skip
// 12 = draw 2
// 13 = pick color
// 14 = pick color + draw 4
typedef struct card{
  char color;
  size_t value;
  struct card* next;
} card;

// deck of cards represented by a linked list
typedef struct {
  card* head;
  size_t size;
} deck;

// Struct representing a player
typedef struct player{
  size_t number;
  deck hand;
  int sock_num;
  struct player* next;
  struct player* prev;
} player;

// Struct representing player turns (possible global)
typedef struct {
  player* head;
  player* cur;
  size_t direction;
} order;

// Struct representing game struct
typedef struct {
  size_t start;
  deck main;
  size_t number_players;
  player turn;
  deck draw;
  deck discard;
  order* player_list;
  size_t end;

} game_state;

/*
 * Make a new card given the color and value of the card
 *
 * Given an char value to for color and size_t value for the card, create a new
 * card on the heap holding that value and return a pointer to that node. The
 * caller of this function is considered the owner of the new node and is
 * responsible for cleaning it up when done with it.
 *
 * @param color The color for the new card.
 * @param value The value for the new card.
 * @return A pointer to the newly created card.
 */

card* make_card(char color, size_t value);

/*
move card
*/
void move_card(card* card, deck* old_deck, deck* new_deck);

/**
 * Free a cards's memory.
 *
 * Given a pointer to the card that was allocated on the heap, free the memory
 * taken up by that node. Generally, this function should only be called in
 * functions that originally allocated the card with make_card, or as part of
 * another free_* function whose corresponding make_* function and/or other
 * functions allocate nodes.
 *
 * If a card not allocated on the heap is attempted to be freed, the result is
 * undefined. Attempting to free a non-node, attempting to free a node that has
 * already been freed, and accessing a node after freeing will all result in
 * undefined behavior.
 *
 * @param card_ A pointer to the card to free.
 */

void free_card(card* card_);

/**
 * Free a decks's memory.
 *
 * Given a pointer to a deck that was allocated on the heap, free the memory
 * taken up by that deck. Any cards contained in the deck will be freed, as will
 * the deck itself. Undefined behavior results if the passed pointer does not
 * point to a list allocated on the heap or has already been freed. Attempting
 * to access the list after it has been freed also results in undefined
 * behavior.
 *
 * @param deck_ A pointer to the deck to free.
 */
void free_deck(deck* deck_);

/**
 * Switch the main card with the new card chosen by player
 *
 * Given a pointer to the game state, update the main card based on the card the player played. 
 * The function will update the player's hand to include one less card as well as move the main card
 * to the discard pile. 
 *
 * @param state A pointer to current game state. 
 * @param new A pointer to the card the player played . 
 */
void switch_main_card(game_state* state, char col, size_t val);

/*
Make new player
*/
player* make_player(int number);
/*
Delete Player
*/
void delete_player(player* user);

/*
 * Create a new empty deck.
 *
 * Create a new deck that is empty (that is, it contains no cards and has a
 * size of zero). The memory for the new list is allocated on the heap.
 *
 * @return A pointer to the newly created deck.
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
void update_moves(game_state* var);

/**
 * Refill the draw pile with a shuffled discard pile. 
 * 
 * @param var A pointer to the variable game state struct.
 *
 */
void refill_draw(game_state* var);

/**
 * Shuffle cards in a deck in place. 
 *
 * The deck of cards will be shuffled in place by moving an randomly selected node to the end of list 
 * 40 times. 
 * 
 * @param deck_ a deck of cards to by randomly shuffled. 
 */
void random_cards(deck* deck_);


/**
 * Update turn based on move
 * @param var A pointer to the variable game state struct.
 *
 */
void update_turn(game_state* var);

/**
 * check UNO
 *
 * @param var A pointer to the variable game state struct.
 */
int check_uno(game_state* var);

/**
 * check end
 *
 * @param var A pointer to the variable game state struct.
 */
int check_win(game_state *var);

void update_player_turn(game_state* var);

card* get_card_index(deck* deck_, size_t index);

void append_card(deck* deck_, char col, size_t val);

player* make_player(int number);

void free_player(player* player_);

order* make_order(int num_players);

void free_order(order* order_);

void append_order(order* order_, player* player_);
