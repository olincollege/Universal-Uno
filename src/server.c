#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif
#define _GNU_SOURCE

#include "server.h"

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "model.h"
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
  struct sockaddr client_addr;
  unsigned int address_size = sizeof(server->addr);
  int connected_d = accept(server->listener, &client_addr, &address_size);

  if (connected_d < 0) {
    error_and_exit("Connection to server failed");
  }

  if (game_state->current_players == 0) {
    // If the player who joined is the first player, it initializes the game
    // state
    game_state->number_players = 2;
    game_state->player_list = make_order(game_state->number_players);
    game_state->player_list->head->sock_num = connected_d;
    printf("Player %zu connected!\n", game_state->player_list->head->number);
    game_state->current_players++;
  } else {
    player* current = game_state->player_list->head;

    for (size_t i = 0; i < game_state->number_players; i++) {
      // Adds the player to the player list
      if (current->sock_num != -1) {
        printf("print socket num for player %zu: %i\n", current->number,
               current->sock_num);
        current = current->next;
      } else {
        printf("Player %zu connecting with socket number %d\n", current->number,
               connected_d);
        current->sock_num = connected_d;

        game_state->current_players = game_state->current_players + 1;
        printf("Player %zu connected! \n", current->number);
        break;
      }
    }
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    while (game_state->current_players != game_state->number_players) {
      // Waits for the right number of players to join.
      printf("Not enough players yet!\n");
      break;
    }

    if (game_state->current_players == game_state->number_players) {
      if (game_state->start != 1) {
        // Starts the game once the proper number of students have joined.
        game_state->start = 1;
        game_state->turn = game_state->player_list->head;
        start_game(game_state);
      }
      play_game(game_state, server);

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
    // Initializes the hands for each player
    make_hand(state, current);

    card* card_ = current->hand.head;
    printf("Player %zu hand\n", current->number);
    while (card_ != NULL) {
      printf("Color: %c ", card_->color);
      printf("Number: %zu\n", card_->value);
      card_ = card_->next;
    }
    printf("\n");
    current = current->next;
  }
  state->start = 1;
  printf("Sending message\n");
  send_message(state, 0, NULL);
}
void play_game(game_state* state) {
  printf("game starting\n");
  
  while (1) {
    printf("at top of while\n");
    // FILE* sock_file = get_socket_file(state->turn->sock_num);
    char buf[BUF_SIZE];
    ssize_t val = read(state->turn->sock_num, buf, BUF_SIZE);

    // fflush()

    if(val < 0) {
      printf("error reading\n");
      errnum = errno;
      printf("Error: %s\n", strerror(errnum));
      break;
    } else if (val == 0) {
      printf("player disconnected\n");
      return;
    }

    printf("read! %s", buf);
    if (buf[0] == 'u') {
      if (check_uno(state) == 1) {
        send_message(state, 1, buf);
      }
    } else if (strstr(buf, "draw") != NULL) {
      printf("Card requested\n");
      if (check_draw(state) == 1) {
        refill_draw(state);
      }
      draw_card(state);
      change_turn(state);

      printf("\n");
      send_message(state, 0, NULL);
    } else {
      printf("playing card\n");
      char card_str[FIVE];
      process_input(buf, card_str);
      play_uno(state, card_str);
      change_turn(state);
      send_message(state, 0, NULL);
      // free(buf);
    }
    // (void)fflush(buf);
  }
}

void send_message(game_state* state, int type, char* buf) {
  printf("at top of send message\n");
  if (type == 1) {
    // If it is a broadcast message, it constructs and send it to everyone
    if (buf[0] == 'u') {
      if (state->turn->hand.size == 1) {
        player* current = state->player_list->head;
        char sendline[1000];
        sprintf(sendline, "Player %zu has called uno!");
        write(current->sock_num, sendline, sizeof(sendline));
        current = current->next;
        while (current != state->turn) {
          char sendlinee[1000];
          sprintf(sendline, "Player %zu has called uno!");
          write(current->sock_num, sendline, sizeof(sendline));
          current = current->next;
        }
      } else {
        player* current = state->player_list->head;
        char sendline2[1000];
        sprintf(sendline2, "A Player has called uno before another!");
        write(current->sock_num, sendline2, sizeof(sendline2));
        current = current->next;
        while (current != state->turn) {
          char sendlinee2[1000];
          sprintf(sendlinee2, "A Player has called uno before another!");
          write(current->sock_num, sendlinee2, sizeof(sendlinee2));
          current = current->next;
        }
      }
    }
  } else {
    // If it is not a broadcast then it simply collects the player info and
    // transmits it
    player* current_player = state->player_list->head;

    for (size_t i = 0; i < state->number_players; i++) {
      char sendlin[1000];
      sprintf(sendlin, "%d/%d/%d/", 0, current_player->number,
              state->turn->number);
      // FILE* input_file = fdopen(current_player->sock_num, "r+");

      char* main_card[100];
      printf("setting main cardsdfsdf\n");
      if (state->main.head == NULL) {
        puts("TEST");
      }
      printf("color %c, val: %d\n", state->main.head->color,
             (int)state->main.head->value);
      sprintf(main_card, " %c%zu/", state->main.head->color,
              state->main.head->value);
      printf("%s", main_card);
      printf("FJFHDJ\n");
      strcat(sendlin, main_card);
      char* num_cards[8];
      sprintf(num_cards, "%d/", state->number_players);
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
      write(current_player->sock_num, sendlin, 1000);
      printf("wrote\n");
      // printf("fputs after\n");
      current_player = current_player->next;
    }
  }
}
