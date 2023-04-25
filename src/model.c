#include "model.h"


card* make_card(char col, size_t val) {
  card* new_card = malloc(sizeof(card));
  new_card->color = col;
  new_card->value = val;
  new_card->next = NULL;
  return new_card;
}

void free_card(card* card_) { free(card_); }

deck* make_deck(void) {
  deck* new_deck = malloc(sizeof(deck));
  new_deck->head = NULL;
  new_deck->size = 0;
  return new_deck;
}

void free_deck(deck* deck_) {
  card* current = deck_->head;
  card* next = NULL;
  while (current != NULL) {
    next = current->next;
    free_card(current);
    current = next;
  }
  free(deck_);
}
