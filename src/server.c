#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE

#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"

uno_server* make_uno_server(struct sockaddr_in ip_addr, int max_backlog) {
  uno_server* server = malloc(sizeof(uno_server));
  server->listener = open_tcp_socket();
  server->addr = ip_addr;
  server->max_backlog = max_backlog;
  return server;
}

void free_uno_server(uno_server* server) {
  close_tcp_socket(server->listener);
  free(server);
}

void listen_for_connections(uno_server* server) {
  if (bind(server->listener, &server->addr, sizeof(server->addr)) == -1) {
    error_and_exit("Binding error");
  }
  if (listen(server->listener, server->max_backlog) == -1) {
    error_and_exit("Listening error");
  };
}

int accept_client(uno_server* server, game_state* game_state) {
  printf("helo\n");
  struct sockaddr_storage client_addr;
  unsigned int address_size = sizeof(client_addr);
  int connected_d =
      accept(server->listener, (struct sockaddr*)&client_addr, &address_size);
  printf("connect_d: %i\n", connected_d);
  if (connected_d == -1) {
    error_and_exit("Connection to server failed");
  }
  if (game_state->current_players == 0) {
    printf("in if state\n");
    game_state->number_players = 2;
    game_state->player_list = *make_order(game_state->number_players);
    game_state->player_list.head->sock_num = connected_d;
    printf("player %zu connected!", game_state->player_list.head->number);
    game_state->current_players++;
  } else {
    printf("in else\n");
    // Maybe when making players initialize # to the number in the array it is,
    // Deck to NULL and sock_num to NULL

    player* current = game_state->player_list.head;
    // while(current->next->sock_num != -1) {
    //   current = current->next;
    // }
    for (int i = 0; i < game_state->number_players; i++) {
      if (current->sock_num != -1) {
        printf("print socket num for player %zu: %i\n", current->number, current->sock_num);
        current = current->next;
        // continue;
      } else {
        current->sock_num = connected_d;
      
        game_state->current_players = game_state->current_players + 1;
        printf("player %zu connected!", current->number);
        // current = current->next;
        break;
      }
      
    }
  }
  printf("forking\n");
  printf("num players: %zu \n", game_state->current_players);
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    // Maybe get input from the first player how many people to expect.
    while (game_state->current_players != game_state->number_players) {
      printf("stuck here\n");
      // Maybe store the current number of players somewhere?
      // View could have a function in the waiting screen to show how many
      // players we are waiting for.
      break;
    }
    // In the view maybe a while loop that checks if start is != 1 so then when
    // it is we switch the clients view to a screen where cards get passed out.
    if(game_state->current_players == game_state->number_players){
      if (game_state->start != 1) {
            game_state->start = 1;
            game_state->turn = game_state->player_list.head;
            start_game(*game_state);
          }
          // play_uno(game_state);
          // send_message(game_state);
          play_game(game_state);

          return -1;
    }
    
  }
  printf("\n");
  return 0;
}

void start_game(game_state game_state) {
  player* current = game_state.player_list.head;
  for (size_t i = 0; i < game_state.number_players; i++) {
    make_hand(game_state.draw.head, current);
    current = current->next;
  }
  game_state.start = 1;
  printf("about to send message?\n");
  send_message(game_state);
  printf("message sent!\n");
}

void play_game(game_state* state) {
  printf("game starting\n");
  char buf[1000];
  while(1) {
    printf("at top of while\n");
    ssize_t val = read((int) state->turn->sock_num, buf, 1000);
    printf("%s\n", buf);
    if(val < 0) {
      printf("Failed to read from Player %i", (int) state->turn->number);
      exit;
    }
    if(val == 0) {
      close_tcp_socket((int) state->turn->sock_num);
      break;
    }

    printf("checking if make here\n");
    if(buf[0] == 'u') {
      printf("inside u if\n");
      if(check_uno(state) == 1){
       //broadcast that uno has been called
       printf("uno called\n");
      } 
    } else if(strstr(buf, "draw") != NULL) {
        printf("Card requested\n");
        card* card_ = state->turn->hand.head;
        while(card_ != NULL){
          printf("Color: %c ", card_->color);
          printf("Number: %i\n", card_->value);
          card_ = card_->next;
        }
        if(check_draw(state) == 1) {
          refill_draw(state);
        }
        draw_card(state);
        card_ = state->turn->hand.head;
        printf("\n");
        while(card_ != NULL){
          printf("Color: %c ", card_->color);
          printf("Number: %i\n", card_->value);
          card_ = card_->next;
        }
        
        send_message(*state);
      } else {
        printf("playing card\n");
        char card_str[5];
        process_input(buf, card_str);
        play_uno(state, card_str);
        change_turn(state);
    }
  
    
  }
}
// void send_hand(game_state game_state) {
//   char* hands[1000];
//   player* current_player = game_state.player_list->head;
//   for (size_t i = 0; i < game_state.number_players; i++) {
//     FILE* input_file = fdopen(current_player->sock_num, "r+");
//     card* current_card = current_player->hand.head;
//     strcat(hands, "{");
//     for (size_t i = 0; i < current_player->hand.size; i++) {
//       char* color = current_card->color;
//       char* card[10];
//       sprintf(card, "[%s%d],", color, current_card->value);
//       strcat(hands, card);
//     }
//     strcat(hands, "}");
//     fputs(hands, input_file);
//   }
// }

// void send_game(game_state game_state) {
//   player* current_player = game_state.player_list->head;
//   char* data[1000];
//   char* size[50];
//   for (size_t i = 0; i < game_state.number_players; i++) {
//     char* size_value[5];
//     sprintf(size_value, "%d,", current_player->hand.size);
//     strcat(size, size_value);
//     current_player = current_player->next;
//   }
//   char* top[5];
//   sprintf(top, "%s%i", game_state.main.head->color,
//           game_state.main.head->color);
//   sprintf(data, "Sizes:%s Top:%s Turn:%i", size, top, game_state.turn);
//   for (size_t i = 0; i < game_state.number_players; i++) {
//     FILE* input_file = fdopen(current_player->sock_num, "r+");
//     fputs(data, input_file);
//     current_player = current_player->next;
//   }
// }

// void send_initial(game_state game_state) {
//   player* current_player = game_state.player_list->head;
//   for (size_t i = 0; i < game_state.number_players; i++) {
//     FILE* input_file = fdopen(current_player->sock_num, "r+");
//     char* id[5];
//     sprintf(id, "%i", current_player->number);
//     fputs(id, input_file);
//     current_player = current_player->next;
//   }
// }

void send_message(game_state game_state) {
  // if error 1:
  // broadcast message.
  // else
  player* current_player = game_state.player_list.head;

  for (size_t i = 0; i < game_state.number_players; i++) {
    char* sendlin[150];
    sprintf(sendlin, "%d/%d/%d/", 0, current_player->number,
            game_state.turn->number);
    FILE* input_file = fdopen(current_player->sock_num, "r+");
    card* current_card = current_player->hand.head;
    char* main[7];
    sprintf(main, " %c%d/", current_card->color, current_card->value);
    strcat(sendlin, main);
    char* num_cards[8];
    sprintf(num_cards, "%d/", game_state.number_players);
    strcat(sendlin, num_cards);
    while (current_card->next != NULL) {
      printf("in loop 1\n");
      char* card[7];
      sprintf(card, " %c%d,", current_card->color, current_card->value);
      strcat(sendlin, card);
      current_card = current_card->next;
    }
    char* num_players[5];
    sprintf(num_players, "/%d/", game_state.number_players);
    strcat(sendlin, num_players);
    player* temp = game_state.player_list.head;
    while (temp->next != game_state.player_list.head) {
      printf("in loop2\n");
      char* hand_size[7];
      sprintf(hand_size, "%d,", temp->hand.size);
      strcat(sendlin, hand_size);
      temp = temp->next;
    }
    printf("fputs before\n");
    fputs(sendlin, input_file);
    printf("fputs after\n");
  }
}

void get_hand_size(player* player, FILE* file) {
  if (player->hand.size < 10) {
    putw(0, file);
    putw(player->hand.size, file);
  } else {
    putw(player->hand.size, file);
  }
}
void send_message_check(game_state game_state) {
  player* current_player = game_state.player_list.head;
  FILE* input_file;
  for (size_t i = 0; i < game_state.number_players; i++) {
    putw(0, input_file);
    putw(current_player->number, input_file);
    putw(game_state.turn->number, input_file);

    get_hand_size(current_player, input_file);
    card* current_card = current_player->hand.head;
    while (current_card->next != NULL) {
      fputs(current_card->color, input_file);
      putw(current_card->value, input_file);
    }
    // putw(game_state.number_players, input_file);
    player* temp = game_state.player_list.head;
    while (temp->next != game_state.player_list.head) {
      get_hand_size(temp, input_file);
      temp = temp->next;
    }
  }
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  if (getline(&recv_line, &recv_line_size, input_file) == -1) {
    // return -1;
  }
  puts(recv_line);
}
