#include "model.h"

card* make_card(char col, size_t val) {
  card* new_card = malloc(sizeof(card));
  //printf("color %c value %zu\n", col, val);
  strncpy (&new_card->color, &col, sizeof(col) );
  //printf("%c\n", new_card->color);
  new_card->value = val;
  //printf("%zu\n", new_card->value);
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

void move_card(card* card_, deck* old_deck, deck* new_deck) {
  // Find card in old deck
  card* prev = old_deck->head;
  // If card is head
  if (prev == card_) {
    old_deck->head = old_deck->head->next;
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

deck* make_UNO_deck(void) {
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

void shuffle(deck* deck_) {
  for (size_t i = 0; i < 54; i++) {
    size_t index = (size_t)(rand() % (UNO_DECK - 1));
    card* swap = get_card_index(deck_, index);
    // Move card from current place to end of list
    move_card(swap, deck_, deck_);
  }
}

card* get_card_index(deck* deck_, size_t index) {
  card* current = deck_->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }
  return current;
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

int check_draw(game_state* state) {
  if (state->discard.size == 0) {
    return 1;
  }
  return 0;
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

void refill_draw(game_state* state) {
  shuffle(&(state->discard));
  append_deck(&(state->discard), &(state->draw));
}

void switch_main_card(game_state* state, char col, size_t val) {
  // Find card
  card* swap = find_card(&(state->turn.hand), col, val);
  move_card((state->main.head), &(state->main), &(state->discard));
  move_card(swap, &(state->turn.hand), &(state->main));
}

player* make_player(int number) {
  player* player_ = malloc(sizeof(player));
  player_->hand = *(make_deck());
  player_->next = NULL;
  player_->prev = NULL;
  player_->sock_num = 0;
  player_->number = number;
  return player_;
}

void free_player(player* player_) { free(player_); }

order* make_order(int num_players) {
  order* new_order = malloc(sizeof(order));
  player* head = make_player(0);
  new_order->head = head;
  new_order->cur = NULL;
  new_order->direction = 0;

  for (size_t i = 1; i < num_players; i++) {
    head->next = make_player(i);
  }
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

void append_order(order* order_, player* player_) {
  if (order_->head == NULL) {
    order_->head = player_;
  } else {
    player* current = order_->head;
    while (current->next != order_->head) {
      current = current->next;
    }
    current->next = player_;
    player_->next = order_->head;
    player_->prev = current;
  }
}
