#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define useful constants needed in the program.
enum {
  /** Number of cards for UNO.*/
  UNO = 1,
  /** Maximum numbers of players*/
  MAX_PLAYERS = 5,
  /** Number of cards in a UNO deck*/
  UNO_DECK = 108,
  /** Special Number for Draw 4*/
  SPECIAL_13 = 13,
  /** Special Number for Wild Draw 4*/
  SPECIAL_14 = 14

};

// Define game state structs.

// A card represented by the color of the card and value.
// Special moves are higher numbers.
// 10 = Reverse
// 11 = Skip
// 12 = Draw 2
// 13 = Wild card
// 14 = Wild Draw 4
typedef struct card {
  char color;
  size_t value;
  struct card* next;
} card;

// A deck of cards represented by a linked list including
// the head of the first card and size of the deck.
typedef struct {
  card* head;
  size_t size;
} deck;

// A player represented the number of the player, the player's hand of cards,
// the player's socket number, and the next and previous player.
typedef struct player {
  size_t number;
  deck hand;
  int sock_num;
  struct player* next;
  struct player* prev;
} player;

// A order of the players represented by a circular doubly linked list including
// the first player and direction of the current turns. 
//
typedef struct {
  player* head;
  size_t direction;
} order;

// The game struct represented by a boolean tracking the start and end the game,
// the current amount of players, the total amount of players, the main card,
// the draw and discard pile, the player list, and the current player's turn.
typedef struct {
  size_t start;
  size_t current_players;
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
 * Given an char value for color and size_t value for the card, create a new
 * card on the heap holding those values and return a pointer to that card. The
 * caller of this function is considered the owner of the new card and is
 * responsible for cleaning it up when done with it.
 *
 * @param color The color for the new card.
 * @param value The value for the new card.
 * @return A pointer to the newly created card.
 */
card* make_card(char color, size_t value);


/**
 * Free a cards's memory.
 *
 * Given a pointer to the card that was allocated on the heap, free the memory
 * taken up by that card. Generally, this function should only be called in
 * functions that originally allocated the card with make_card, or as part of
 * another free_* function whose corresponding make_* function and/or other
 * functions allocate cards.
 *
 * If a card is not allocated on the heap is attempted to be freed, the result is
 * undefined. Attempting to free a non-card, attempting to free a card that has
 * already been freed, and accessing a card after freeing will all result in
 * undefined behavior.
 *
 * @param card_ A pointer to the card to free.
 */
void free_card(card* card_);

/*
 * Create a new empty deck.
 *
 * Create a new deck that is empty (that is, it contains no cards and has a
 * size of zero). The memory for the new deck is allocated on the heap.
 *
 * @return A pointer to the newly created deck.
 */
deck* make_deck(void);

/**
 * Free a decks's memory.
 *
 * Given a pointer to a deck that was allocated on the heap, free the memory
 * taken up by that deck. Any cards contained in the deck will be freed, as will
 * the deck itself. Undefined behavior results if the passed pointer does not
 * point to a deck allocated on the heap or has already been freed. Attempting
 * to access the deck after it has been freed also results in undefined
 * behavior.
 *
 * @param deck_ A pointer to the deck to free.
 */
void free_deck(deck* deck_);

/**
 * Append a card to a deck.
 *
 * Given a deck and a card color and value, add that card to the end of the deck. This
 * function modified the deck in place (i.e., it does not create a new copy of
 * the deck to append to).
 *
 * @param deck__ A pointer to the deck to append to.
 * @param color The color for the new card.
 * @param value The value for the new card.
 */
void append_card(deck* deck_, char col, size_t val);

/*
 * Create a new UNO deck.
 *
 * Create a new UNO deck that is has 108 cards with the standard card values in an UNO
 * deck including correct color and value. The memory for the UNO deck is allocated on
 * the heap.
 *
 * @return A pointer to the newly created UNO deck.
 */

deck* make_uno_deck(void);

/**
 * Move a card from the original deck to the end of a different deck
 * 
 * Given a card, the deck the card was in, and the deck to move the card to, move the card 
 * from one deck to the end of another. This function modified each deck in place and does not create a new card.
 * The function fixes the linked list connections of the original deck and decreases the size by one, and increases the size
 * of the second deck by 1 after adding the card to the end.
 * 
 * The function has undefined behavior if the card, old_deck or new_deck does not exist. The function
 * also has undefined behavior if the card is not found in the old_deck. 
 * 
 * @param card_ A pointer to the card to be moved.
 * @param old_deck A pointer to the deck the card was originally in.
 * @param new_deck A pointer to the deck to move the card into. 
 */
void move_card(card* card_, deck* old_deck, deck* new_deck);

/**
 * Get a card given deck the card is in and the index of the card. 
 * 
 * Given a deck and a index that is less than the size of the deck return a pointer to the card at
 * the specific index. If the index is outboards of the deck, the function returns NULL.
 * 
 * The function has undefined behavior if the deck_ does not exist.
 * 
 * @param deck_ A pointer to the the deck the card is in.
 * @param index A size_t integer of the index of the card to get.
 */
card* get_card_index(deck* deck_, size_t index);

/**
 * Shuffle cards in a deck.
 *
 * The deck of cards will be shuffled in place by moving randomly selected cards
 * end of list. This function modified the deck in place without changing the value of the card
 * or amount of the cards. 
 *
 * @param deck_ a deck of cards to be shuffled.
 */
void shuffle(deck* deck_);

/**
 * Check whether the contents of two decks are equal.
 *
 * Given two decks, check whether they are equal (i.e., their lengths are the
 * same and they contain the same cards in the same order). The deck can point
 * to the same location, in which case they will be equal by definition, but
 * only the card values (not the memory locations of the deck/cards) are compared.
 * This function is a helper function for testing card and deck functions. 
 *
 * @param lhs A pointer to one of the decks to compare.
 * @param rhs A pointer to the other deck to compare.
 * @return 1 if the decks are equal in value and 0 otherwise.
 */
int equal(deck* deck_1, deck* deck_2);

/**
 * Find a card in a deck based on the color and value of the card
 *
 * Given a pointer to a deck, find and return a pointer to a card that matches the
 * inputted color and value 
 *
 * @param deck__ A pointer to the deck to append to.
 * @param color The color for the new card.
 * @param value The value for the new card.
 * @return a pointer to the card found or a NULL pointer if the card is not found
 */
card* find_card(deck* deck_, char color, size_t value);

/**
 * Append a deck to another deck.
 *
 * Given a pointer to the old deck and a pointer to a new deck, set the
 * head of the old deck to the final value of the new deck, then set the old deck head 
 * to NULL and the size to 0. The function does not create any new cards or decks, only moves
 * one deck to the end of another. 
 *
 * @param deck_old A pointer to the deck to be moved.
 * @param deck_new A pointer to the deck to be appended to.
 */
void append_deck(deck* deck_old, deck *deck_new);

/**
 * Check the size of the draw pile 
 *
 * Given a pointer to the game state, check the state of the draw pile. If the draw pile is greater than 1 card, 
 * return 0 to indicate it does not needs to be refilled. If the draw pile is 1 or below, return 1 to indicate 
 * the draw pile needs to be refilled. 
 *
 * @param state A pointer to the current game state.
 * @return 1 if the draw pile needs to refilled or 0 if the draw pile does not need to be 
 * refilled. 
 */
int check_draw(game_state* state);

/**
 * Refill the draw pile.  
 *
 * Given a pointer to the game state, refill the draw pile by shuffling the discard pile, and 
 * moving the cards to the end of the draw pile. The discard pile should be empty after this function 
 * is run, and no cards or decks are created. 
 *
 * @param state A pointer to the current game state.
 */
void refill_draw(game_state* state);

/**
 * Move a card from the draw pile to player's hand. 
 *
 * Given a pointer to the game state, move the top card from the draw pile to the current
 * play's hand. 
 *
 * @param state A pointer to the current game state.
 */
void draw_card(game_state* state);

/**
 * Switch the main card with the new card chosen by the player.
 *
 * Given a pointer to the game state, update the main card based on the card the
 * player played. The function will update the player's hand to include one less
 * card as well as move the main card to the discard pile.
 *
 * @param state A pointer to current game state.
 * @param new A pointer to the card the player played .
 */
void switch_main_card(game_state* state, char col, size_t val);

/**
 * Make a new player given the player's number.
 *
 * Given an number for the player, create a new player on the heap holding the player's number,
 * an empty deck for the player's hand, and the other player values set to NULL or -1. The function
 * returns a pointer to that player. The caller of this function is considered the owner of the new
 * player and is responsible for cleaning it up when done with it.
 *
 * @param number The number of the player.
 * @return A pointer to the newly created player.
 */
player* make_player(size_t number);

/**
 * Free a player's memory.
 *
 * Given a pointer to the player that was allocated on the heap, free the memory
 * taken up by that player. The function should also free the player's hand which 
 * was also allocated on the heap. Generally, this function should only be called in
 * functions that originally allocated the player with make_player, or as part of
 * another free_* function whose corresponding make_* function and/or other
 * functions allocate cards.
 *
 * If a player is not allocated on the heap is attempted to be freed, the result is
 * undefined. Attempting to free a non-player, attempting to free a player that has
 * already been freed, and accessing the player after freeing will all result in
 * undefined behavior.
 *
 * @param player_ A pointer to the player to free.
 */
void free_player(player* player_);

/**
 * Make a new order given the number of players.
 *
 * Given an number of players, create a new order on the heap and return a pointer to that order. The order will 
 * create a circular doubly linked list of players. The caller of this function is considered the owner of the new
 * order and is responsible for cleaning it up when done with it.
 *
 * @param num_players The number of the players to create the order with. 
 * @return A pointer to the newly created order.
 */
order* make_order(size_t num_players);

/**
 * Free a orders's memory.
 *
 * Given a pointer to a order that was allocated on the heap, free the memory
 * taken up by that order. Any players contained in the deck will be freed, as will
 * the order itself. Undefined behavior results if the passed pointer does not
 * point to a order allocated on the heap or has already been freed. Attempting
 * to access the order after it has been freed also results in undefined
 * behavior.
 *
 * @param order_ A pointer to the order to free.
 */
void free_order(order* order_);

/**
 * Check if the current player has uno
 * 
 * Give a pointer to the state of the game, check if the current player has 1 card left. 
 *
 * @param state A pointer to the current game state.
 * @return 1 if the player has uno or 0 if the player does not have uno.
 */
int check_uno(game_state* state);

/**
 * Check if the current player has won the game. 
 * 
 * Given a pointer to the state of the game check if the current player has zero cards left 
 * and has won the game.
 *
 * @param state A pointer to the current game state.
 * @return 1 if the player has won the game or 0 if the player has not won. 
 */
int check_win(game_state* state);









