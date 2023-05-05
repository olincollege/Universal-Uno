#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE



#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "model.h"

#include "server.h"
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
  printf("at top of accept client\n");
  struct sockaddr client_addr;
  unsigned int address_size = sizeof(server->addr);
  int connected_d =
      accept(server->listener, &client_addr, &address_size);

  if (connected_d < 0) {
    error_and_exit("Connection to server failed");
  }

  if (game_state->current_players == 0) {
    game_state->number_players = 2;
    game_state->player_list = make_order(game_state->number_players);
    game_state->player_list->head->sock_num = connected_d;
    printf("Player %zu connecting with socket number %d\n", game_state->player_list->head->number, connected_d);
    game_state->current_players++;
  } else {
    
    player* current = game_state->player_list->head;
    
    for (size_t i = 0; i < game_state->number_players; i++) {
      if (current->sock_num != -1) {
        printf("print socket num for player %zu: %i\n", current->number,
               current->sock_num);
        current = current->next;
      } else {
        printf("Player %zu connecting with socket number %d\n", current->number, connected_d);
        current->sock_num = connected_d;

        game_state->current_players = game_state->current_players + 1;
        printf("Player %zu connected! \n", current->number);
        break;
      }
    }
  }
  // printf("forking\n");
  // printf("num players: %zu \n", game_state->current_players);
  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    // Maybe get input from the first player how many people to expect.
    while (game_state->current_players != game_state->number_players) {
      printf("Not enough players yet!\n");
      break;
    }

    if (game_state->current_players == game_state->number_players) {
      if (game_state->start != 1) {
            game_state->start = 1;
            game_state->turn = game_state->player_list->head;
            start_game(game_state);
          }
          play_game(game_state);

      return -1;
    }
  }
  printf("\n");
  return 0;
}

void start_game(game_state* state) {
  printf("in start game\n");
  player* current = state->player_list->head;
  for (size_t i = 0; i < state->number_players; i++) {
    make_hand(state, current);
    
    card* card_ = current->hand.head;
    printf("Player %zu hand\n", current->number);
    while(card_ != NULL){
      printf("Color: %c ", card_->color);
      printf("Number: %zu\n", card_->value);
      card_ = card_->next;
    }
    printf("\n");
    current = current->next;
  }
  state->start = 1;
  printf("Sending message\n");
  send_message(state);
}
void play_game(game_state* state) {
  printf("game starting\n");
  
  while (1) {
    printf("at top of while\n");
    // FILE* sock_file = get_socket_file(state->turn->sock_num);
    char buf[1000];
    ssize_t val = read(state->turn->sock_num, buf, 1000);
    if(val < 0) {
      printf("error reading\n");
      exit;
    } else if(val == 0) {
      printf("player disconnected\n");
      exit;
    }

    printf("read! %s", buf);
    if (buf[0] == 'u') {
      if (check_uno(state) == 1) {
        send_message(state);
      }
    } else if (strstr(buf, "draw") != NULL) {
      printf("Card requested\n");
      if (check_draw(state) == 1) {
        refill_draw(state);
      }
      draw_card(state);
      change_turn(state);

      printf("\n");
      send_message(state);
    } else {
      printf("playing card\n");
      char card_str[5];
      process_input(buf, card_str);
      play_uno(state, card_str);
      change_turn(state);
      send_message(state);
      // free(buf);
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

void send_message(game_state* state) {
  printf("at top of send message\n");
  // if error 1:
  // broadcast message.
  // else
  player* current_player = state->player_list->head;

  for (size_t i = 0; i < state->number_players; i++) {
    char sendlin[1000];
    sprintf(sendlin, "%d/%d/%d/", 0, current_player->number,
            state->turn->number);
    // FILE* input_file = fdopen(current_player->sock_num, "r+");

    char* main_card[100];
    printf("setting main card\n");
    if (state->main.head == NULL) {
      puts("TEST");
    }
    printf("color %c, val: %d\n", state->main.head->color,
           (int)state->main.head->value);
    sprintf(main_card, " %c%zu/", state->main.head->color,
            state->main.head->value);
    printf("%s", main_card);
    strcat(sendlin, main_card);
    char* num_cards[8];
    sprintf(num_cards, "%d/", state->turn->hand.size);
    strcat(sendlin, num_cards);
    printf("about to enter while loop\n");
    card* current_card = current_player->hand.head;
    while (current_card != NULL) {
      // printf("in loop 1\n");
      char* card[7];
      sprintf(card, " %c%zu,", current_card->color, current_card->value);
      strcat(sendlin, card);
      current_card = current_card->next;
    }
    char* num_players[5];
    sprintf(num_players, "/%d/", state->number_players);
    strcat(sendlin, num_players);
    player* temp = state->player_list->head;
    char* hand_size[7];
    sprintf(hand_size, "%zu/", temp->hand.size);
    strcat(sendlin, hand_size);
    temp = temp->next;
    while (temp != state->player_list->head) {
      // printf("in loop2\n");
      char* hand_sizee[7];
      sprintf(hand_sizee, "%zu/", temp->hand.size);
      strcat(sendlin, hand_sizee);
      temp = temp->next;
    }
    // strcpy(sendlin, "hello");
    strcat(sendlin, "\0");
    // printf("fputs before\n");
    printf("about to send input\n");
    printf("%s\n", sendlin);
    // fputs(sendlin, input_file);
    if(write(current_player->sock_num, sendlin, 1000) < 0) {
      error_and_exit("Didn't write successfully\n");
    }
    printf("wrote\n");
    // printf("fputs after\n");
    current_player = current_player->next;
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
  player* current_player = game_state.player_list->head;
  FILE* input_file;

  for (size_t i = 0; i < game_state.number_players; i++) {
    putw(0, input_file);
    putw(current_player->number, input_file);
    putw(game_state.turn->number, input_file);

    get_hand_size(current_player, input_file);
    card* current_card = current_player->hand.head;
    while (current_card->next != NULL) {
      fputs(&current_card->color,
            input_file);  // note is invalid, color is character not string
      putw(current_card->value, input_file);
    }
    // putw(game_state.number_players, input_file);
    player* temp = game_state.player_list->head;
    while (temp->next != game_state.player_list->head) {
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
