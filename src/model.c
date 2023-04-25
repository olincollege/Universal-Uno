#include "model.h"


card* make_card(char col, size_t val) {
  card* new_card = malloc(sizeof(card));
  new_card->color = col;
  new_card->value = val;
  new_card->next = NULL;
  return new_card;
}

void free_card(card* card_) { free(card_); }

void append_card(deck* deck_, char col, size_t val) {
  card* new = make_card(col, val);
  if (deck_->head == NULL) {
    deck_->head = new;
  } else {
    card* current = deck_->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new;
  }
  deck_->size++;
}

move_card(card* card_, deck* old_deck, deck* new_deck) {
  //Find card in old deck
  card* prev = old_deck->head;
  //If card is head 
  if(prev == card_){
    old_deck->head = old_deck->head->next;
    old_deck->size = old_deck->size - 1; 
  }else{
  card* current = prev->next;
  while (current != NULL) {
    // Cut out card 
        if (prev->next == card_){
          prev->next = current->next;
          old_deck->size = old_deck->size - 1; 
        }
        prev = current; 
        current = current->next;
    }
  }
   card_->next = NULL;
  if (new_deck->head == NULL) {
    new_deck->head = card_;
  } else {
    card* current = new_deck->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = card_;
  }
  new_deck->size++;
}

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

deck* make_UNO_deck(void){
  deck* UNO_deck = make_deck();
  //Red Colors 
  append_card(UNO_deck, 'R',0); 
  for(size_t i = 0; i<2; i++){
    for(size_t j = 1; i < 13; j++){
      append_card(UNO_deck, 'r',j);
    }
  }
  
  //Blue Colors
  append_card(UNO_deck, 'B',0); 
  for(size_t i = 0; i<2; i++){
    for(size_t j = 1; i < 13; j++){
      append_card(UNO_deck, 'r',j);
    }
  }
  //Yellow Colors 
  append_card(UNO_deck, 'Y',0); 
  for(size_t i = 0; i<2; i++){
    for(size_t j = 1; i < 13; j++){
      append_card(UNO_deck, 'r',j);
    }
  }
  //Green Colors
  append_card(UNO_deck, 'G',0); 
  for(size_t i = 0; i<2; i++){
    for(size_t j = 1; i < 13; j++){
      append_card(UNO_deck, 'r',j);
    }
  }
  //Wild Cards
  for(size_t i = 0; i<4; i++){
      append_card(UNO_deck, 'A',13);
       append_card(UNO_deck, 'A',14);
    }
  return UNO_deck; 
}
