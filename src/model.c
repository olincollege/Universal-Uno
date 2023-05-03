#include "model.h"

card* make_card(char col, size_t val) {
  card* new_card = malloc(sizeof(card));
  strncpy (&new_card->color, &col, sizeof(col) );
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

deck* make_uno_deck(void) {
  deck* UNO_deck = make_deck();
  // zero colors 
  append_card(UNO_deck, 'R', 0);
  append_card(UNO_deck, 'B', 0);
  append_card(UNO_deck, 'Y', 0);
  append_card(UNO_deck, 'G', 0);
  for (size_t i = 0; i < 2; i++) {
    for (size_t j = 1; j < SPECIAL_13; j++) {
      append_card(UNO_deck, 'R', j);
      append_card(UNO_deck, 'G', j);
      append_card(UNO_deck, 'B', j);
      append_card(UNO_deck, 'Y', j);
    }
  }
  // Wild Cards
  for (size_t i = 0; i < 4; i++) {
    append_card(UNO_deck, 'W', SPECIAL_13);
    append_card(UNO_deck, 'W', SPECIAL_14);
  }
  return UNO_deck;
}

void move_card(card* card_, deck* old_deck, deck* new_deck) {
  // Find card in old deck
  card* prev = old_deck->head;
  // If card is head
  if (prev == card_ && prev != NULL) {
    old_deck->head = prev->next;
    old_deck->size = old_deck->size - 1;
  } else {
    card* current = prev->next;
    while (current != NULL) {
      // Cut out card
      if (prev->next == card_) {
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

card* get_card_index(deck* deck_, size_t index) {
  if(index >= deck_->size){
    return NULL;
  }
  card* current = deck_->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }
  return current;
}

void shuffle(deck* deck_) {
  if(deck_->size == 1 || deck_->size == 0){
    return; 
  }
  for (size_t i = 0; i < deck_->size; i++) {
    size_t index = (size_t)(rand() % (deck_->size)); // NOLINT(cert-msc30-c, cert-msc50-cpp,concurrency-mt-unsafe)
    card* swap = get_card_index(deck_, index);
    move_card(swap, deck_, deck_);

  }
}

int equal(deck* deck_1, deck* deck_2) {
  // Lengths are not equal
  if (deck_1->size != deck_2->size) {
    return 0;
  }
  card* card_1 = deck_1->head;
  card* card_2 = deck_2->head;
  while (card_1 != NULL) {
    if (card_1->value != card_2->value || card_1->color != card_2->color) {
      return 0;
    }
    card_1 = card_1->next;
    card_2 = card_2->next;
  }
  return 1;
}

card* find_card(deck* deck_, char col, size_t val) {
  card* current = deck_->head;
  for (size_t i = 0; i < deck_->size; i++) {
    if (current->color == col && current->value == val) {
      return current;
    }
    current = current->next;
  }
  return NULL;
}

void append_deck(deck* original, deck* new) {
  card* head = original->head;
  if (new->head == NULL) {
    new->head = head;
  } else {
    card* current = new->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = head;
  }
  new->size = new->size + original->size;
  original->head = NULL;
  original->size = 0;
}

int check_draw(game_state* state) {
  if (state->draw.size > 1) {
    return 1;
  }
  return 0;
}

void refill_draw(game_state* state) {
  shuffle(&(state->discard));
  append_deck(&(state->discard), &(state->draw));
}

void draw_card(game_state* state){
  move_card(state->draw.head, &state->draw, &state->turn.hand);
}

void switch_main_card(game_state* state, char col, size_t val) {
  // Find card
  card* swap = find_card(&(state->turn.hand), col, val);
  move_card((state->main.head), &(state->main), &(state->discard));
  move_card(swap, &(state->turn.hand), &(state->main));
}

player* make_player(size_t number) {
  player* player_ = malloc(sizeof(player));
  deck* player_hand = make_deck();
  player_->hand = *player_hand;
  player_->next = NULL;
  player_->prev = NULL;
  player_->sock_num = -1;
  player_->number = number;
  return player_;
}

void free_player(player* player_) { 
  free_deck(&player_->hand);
  free(player_); 
  }

order* make_order(size_t num_players) {
  order* new_order = malloc(sizeof(order));
  player* head_ = make_player(0);
  new_order->head = head_;
  new_order->direction = 0;
  // printf("%i\n", new_order->head->number);
  for (size_t i = 1; i < num_players; i++) {
    head_->next = make_player(i);
    head_->next->prev = head_;
    head_ = head_->next;
    // printf("%i\n", head_->number);
  }
  
  new_order->head->prev = head_;
  new_order->head->prev->next = new_order->head;
  return new_order;
}

void free_order(order* order_) {
  player* current = order_->head;
  player* next = NULL;
  while (current != NULL) {
    next = current->next;
    free_player(current);
    current = next;
  }
  free(order_);
}
int check_uno(game_state* state){
  if(state->turn.hand.size == 1){
    return 1;
  }
  return 0; 
}
int check_win(game_state* state){
  if(state->turn.hand.size == 0){
    return 1; 
  }
  return 0; 
}


