#include "controller.h"
void process_input(char* buf, char* processed_input) {
  // char* processed_input[strlen(buf)];

  if(buf[1] == 'r') {
    processed_input[0] = buf[0];
    strcat(processed_input, "10");
  } else if(buf[1] == 'S') {
    processed_input[0] = buf[0];
    strcat(processed_input, "11");
  } else if(buf[1] == '2') {
    processed_input[0] = buf[0];
    strcat(processed_input, "12");
  } else if(buf[0] == 'W' && buf[1] == '4') {
    processed_input[0] = buf[2];
    strcat(processed_input, "14");
  } else if(buf[0] == 'W') {
    processed_input[0] = buf[1];
    strcat(processed_input, "13");
  } else {
    strcpy(processed_input, buf);
  }
}

int in_hand(card* played_card, deck* hand) {
  card* curr_hand_card = hand->head;

  while(curr_hand_card != NULL) {
    if(curr_hand_card->color == played_card->color) {
      if(curr_hand_card->value == played_card->value) {
        return 1;
      }
    }
    curr_hand_card = curr_hand_card->next;
  }
  return 0;
}

int is_valid(char* card_, game_state* game_state_) {
    // turn string into card object
    char* num_str = &card_[1];  
    int num = atoi(num_str);
    card* curr_card = make_card(card_[0], (size_t) num);
    int is_in_hand = in_hand(curr_card, &(game_state_->turn.hand));
    
    if(is_in_hand == 1) {
      if(curr_card->value == DRAW_4) {
        return 1;
      }
      if(curr_card->value == WILD) {
        return 1;
      }
      if(curr_card->color == game_state_->main.head->color) {
        if(curr_card->value == REVERSE) {
            // switch directions, return 1
            return 1;
          } 
          if(curr_card->value == SKIP) {
            // skip, return 1
            return 1;
          } 
          if(curr_card->value == DRAW_2) {
            // draw 2, return 1
            return 1;
          }
        return 1;
      } 
      
      if(curr_card->value == game_state_->main.head->value){
          if(curr_card->value == REVERSE) {
              // switch directions, return 1
              return 1;
            } 
            if(curr_card->value == SKIP) {
              // skip, return 1
              return 1;
            } 
            if(curr_card->value == DRAW_2) {
              // draw 2, return 1
              return 1;
            }
          return 1;
      }
      
    }
    return 0; 
}


