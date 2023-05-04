#include "model.h"

card* make_card(char col, size_t val) {
  card* new_card = malloc(sizeof(card));
  strncpy(&new_card->color, &col, sizeof(col));
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
    for (size_t j = 1; j < DRAW_4; j++) {
      append_card(UNO_deck, 'R', j);
      append_card(UNO_deck, 'G', j);
      append_card(UNO_deck, 'B', j);
      append_card(UNO_deck, 'Y', j);
    }
  }
  // Wild Cards
  for (size_t i = 0; i < 4; i++) {
    append_card(UNO_deck, 'W', DRAW_4);
    append_card(UNO_deck, 'W', WILD);
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
  if (index >= deck_->size) {
    return NULL;
  }
  card* current = deck_->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }
  return current;
}

void shuffle(deck* deck_) {
  if (deck_->size == 1 || deck_->size == 0) {
    return;
  }
  for (size_t i = 0; i < deck_->size; i++) {
    size_t index =
        (size_t)(rand() %
                 (deck_->size));  // NOLINT(cert-msc30-c,
                                  // cert-msc50-cpp,concurrency-mt-unsafe)
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

void append_deck(deck* deck_old, deck* deck_new) {
  card* head = deck_old->head;
  if (deck_new->head == NULL) {
    deck_new->head = head;
  } else {
    card* current = deck_new->head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = head;
  }
  deck_new->size = deck_new->size + deck_old->size;
  deck_old->head = NULL;
  deck_old->size = 0;
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

void draw_card(game_state* state) {
  move_card(state->draw.head, &(state->draw), &(state->turn->hand));
}

void place_card(game_state* state, char col, size_t val) {
  // Find card
  card* swap = find_card(&(state->turn->hand), col, val);
  move_card((state->main.head), &(state->main), &(state->discard));
  move_card(swap, &(state->turn->hand), &(state->main));
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
  player* head = make_player(0);
  new_order->head = head;
  new_order->direction = 0;
  if (num_players == 1) {
    new_order->head->next = new_order->head;
    new_order->head->prev = new_order->head;
    return new_order;
  }
  for (size_t i = 1; i < num_players; i++) {
    head->next = make_player(i);
    player* temp = head;
    head = head->next;
    head->prev = temp;
  }
  new_order->head->prev = head;
  head->next = new_order->head;
  player* heads = new_order->head;
  for (size_t j = 0; j < num_players; j++) {
    heads = heads->next;
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

void make_hand(game_state* state, player* player) {
  for (size_t i = 0; i < SEVEN; i++) {
    draw(state, player);
  }
}
int check_uno(game_state* state) {
  if (state->turn->hand.size == 1) {
    return 1;
  }
  return 0;
}

void switch_direction(game_state* state) {
  if (state->player_list.direction == 0) {
    state->player_list.direction = 1;

  } else {
    state->player_list.direction = 0;
  }
}

void skip(game_state* state) {
  state->turn = state->turn->next->next;
}

void draw(game_state* state, player* player) {
  move_card(state->draw.head, &(state->draw), &player->hand);
}

void draw2(game_state* state, player* player) {
  for (size_t i = 0; i < TWO; i++) {
    draw(state, player);
  }
}

void draw4(game_state* state, player* player) {
  for (size_t i = 0; i < TWO; i++) {
    draw2(state, player);
  }
}

void next_player(game_state* state) {
  if (state->player_list.direction == 1) {
    state->turn = state->turn->prev;
  } else {
    state->turn = state->turn->next;
  }
}

void play_uno(game_state* state, char* input) {
  char number[2];
  strcpy(number, input[1]);
  char switch_num = input[2];
  // strcat(number, switch_num);
  int num = atoi(&number);
  char col = input[0];

  if(is_valid(input, state) == 1) {
    switch (switch_num) {
    case '1':
      switch_direction(state);
      place_card(state, col, num);
      break;
    case '0':
      skip(state);
      place_card(state, col, num);
      break;
    case '2':
      draw2(state, state->turn->next);
      place_card(state, col, num);
      break;
    case '3':
      draw4(state, state->turn->next);
      place_card(state, col, num);
      break;

    default:
      place_card(state, col, num);
      // next_player(state);
      break;
    }
  }

  
  // return 0; 
}

game_state* make_game_state(void) {
  game_state* state = malloc(sizeof(game_state));
  state->discard = *make_deck();
  state->draw = *make_uno_deck();
  shuffle(&(state->draw));
  card* card_ = state->draw.head;
  // for(int i = 0; i < 108; i++) {
  //   printf("Color: %c ", card_->color);
  //   printf("Number: %i\n", card_->value);
  //   card_ = card_->next;
  // }
  state->end = 0;
  state->main = *make_deck();
  state->number_players = 0;
  state->start = 0;
  state->turn = NULL;
  state->current_players = 0;
  // state->player_list = NULL;
  return state;
}

int check_win(game_state* state) {
  if (state->turn->hand.size == 0) {
    return 1;
  }
  return 0;
}

int in_hand(card* played_card, deck* hand) {
  card* curr_hand_card = hand->head;

  while (curr_hand_card != NULL) {
    if (curr_hand_card->color == played_card->color) {
      if (curr_hand_card->value == played_card->value) {
        return 1;
      }
    }
    curr_hand_card = curr_hand_card->next;
  }
  return 0;
}

int is_valid(char* card_, game_state* state) {
    // turn string into card object
    char* num_str = &card_[UNO];  
    int num = atoi(num_str);
    card* curr_card = make_card(card_[0], (size_t) num);
    int is_in_hand = in_hand(curr_card, &(state->turn->hand));
    // make sure card is valid uno card
    if(num > WILD) {
      return 0;
    }
    if(card_[0] != 'B' && card_[0] != 'R' && card_[0] != 'Y' && card_[0] != 'G') {
      return 0;
    }
    if(is_in_hand == UNO) {
      if(curr_card->value == DRAW_4) {
        return 1;
      }
      if(curr_card->value == WILD) {
        return 1;
      }
      if(curr_card->color == state->main.head->color) {
        return 1;
      } 
      
      if(curr_card->value == state->main.head->value){
          return 1;
      }
      
    }
    return 0; 
}

void change_turn(game_state* state) {
  // printf("%i\n", state->turn.number);
  if (state->player_list.direction == 0) {
    // 0 is next instead of prev
    state->turn = (state->turn->next);
  } else {
    // 1 is prev
    state->turn = (state->turn->prev);
  }
}
