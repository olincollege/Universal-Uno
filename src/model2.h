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
  size_t special; 
} card;

// deck of cards represented by a doubly linked list
typedef struct {
  card* head;
  card* prev;
  card* next; 
  size_t size;
} deck;

//Struct representing a player 
typedef struct {
  size_t number;
  deck hand;
  int sock_num; 
} player;

//Struct representing player turns
typedef struct {
  player* loop;
  player* prev;
  player* next;
} turn;

//Struct representing game struct
typedef struct {
  card main;
  int turn; // player number 
  deck draw; 
  deck discard; 
  player* players[PLAYERS];
  size_t end;
} game_state;

/**
 * Shuffle cards
 *
 * @param pile a pile of cards .
 */
void shuffle(card pile[DECK]);
