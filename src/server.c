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

int accept_client(uno_server* server, game_state game_state) {
  struct sockaddr_storage client_addr;
  unsigned int address_size = sizeof(client_addr);
  int connected_d =
      accept(server->listener, (struct sockaddr*)&client_addr, &address_size);
  if (connected_d == -1) {
    error_and_exit("Connection to server failed");
  }
  if (game_state.player_list->head == NULL) {
    game_state.player_list = make_order(game_state.number_players);
  }
  // Maybe when making players initialize # to the number in the array it is,
  // Deck to NULL and sock_num to NULL
  player* current = game_state.player_list->head;
  for (int i = 0; i < game_state.number_players; i++) {
    if (current->sock_num != NULL) {
      continue;
    }
    current->sock_num = connected_d;
    break;
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    // Maybe get input from the first player how many people to expect.
    while (game_state.player_list->head->prev->sock_num == NULL) {
      // Maybe store the current number of players somewhere?
      // View could have a function in the waiting screen to show how many
      // players we are waiting for.
      continue;
    }
    // In the view maybe a while loop that checks if start is != 1 so then when
    // it is we switch the clients view to a screen where cards get passed out.
    if (game_state.start != 1) {
      game_state.start = 1;
      game_state.player_list->cur = game_state.player_list->head;
    }
    uno(game_state, connected_d);
    return -1;
  }

  return 0;
}

int is_valid(char* card[], game_state* game_state_) {
    // turn string into card object
    card curr_card;
    curr_card.color = card[0];
    int num = atoi(card[1]);
    curr_card.value = (size_t) num;
    int in_hand = 0;

    // check if card is in hand
    card* curr_hand_card = game_state_->turn->hand->head;
    while(curr_hand_card != NULL) {
        if(curr_hand_card->color == curr_card.color) {
            if(curr_hand_card->value == curr_card.value) {
                in_hand = 1;
                break;
            }
        }
        curr_hand_card = curr_hand_card->next;
    }

    if(in_hand == 1) {

        if(curr_hand_card->color == game_state_->main.color) {
            // move the card from your hand to the played deck
            move_card(curr_hand_card, &(game_state_->turn->hand), &(game_state_->discard));
            return 1;
        }
        if(curr_hand_card->value == game_state_->main.value) {
            if(curr_hand_card->value == 10) {
              // switch directions, return 1

            } else if(curr_hand_card->value == 11) {
              // skip, return 2
            } else if(curr_hand_card->value == 12) {
              // draw 2, return 3
            } else if(curr_hand_card->value == 14) {
              // wild card, ask for color
            } else if(curr_hand_card->value == 15) {
              //pick color + draw 4
            }
            return 1;
        }
        if(curr_hand_card->value == 14) {
          // send instructions to pick a color
          return 2;

        }
        if(curr_hand_card->value == 15) {
            // add four cards to next player
            // return 2 -- means that more information is needed 
            
            return 2;
        }
    } else {
        // return 0 -- means move was invalid
        return 0;
    }
}

void uno(game_state game_state, int socket_descriptor) {
  if (game_state.player_list->head->hand.size < 7) {
    player* current = game_state.player_list->head;
    for (size_t i = 0; i < game_state.number_players; i++) {
      for (size_t i = 0; i < 7; i++) {
        // move_card(game_state.draw.head, game_state.draw, current->hand);
      }
      current = current->next;
      // After giving all the players the cards, we will send a message letting,
      // each client know what their cards are.
    }
  }

  FILE* input_file = fdopen(socket_descriptor, "r+");
  char* buf = NULL;
  size_t buf_size = 0;
  getline(&buf, &buf_size, input_file);
  char color = buf[0];
  char* number = buf[1];
  strcat(number, buf[2]);

  switch (number[1]) {
    case '0':
      // switch_direction(game_state);
      break;
    case '1':
      // skip(game_state);
      break;
    case '2':
      // draw_two(game_state);
      break;
    case '3':
      // wild card
      break;
    case '4':
      // draw_4(game_state);
      break;

    default:
      // move_card(current_card, game_state.player_list.cur->hand,
      //           game_state.main);
      break;
  }
}

void start_game(game_state game_state, uno_server* server,
                int socket_descriptor) {
  player* current = game_state.player_list->head;
  for (size_t i = 0; i < game_state.number_players; i++) {
    // current->hand = make_hand();
    current = current->next;
  }
  game_state.start = 1;
}

void send_hand(game_state game_state) {
  char* hands[1000];
  player* current_player = game_state.player_list->head;
  for (size_t i = 0; i < game_state.number_players; i++) {
    FILE* input_file = fdopen(current_player->sock_num, "r+");
    card* current_card = current_player->hand.head;
    strcat(hands, "{");
    for (size_t i = 0; i < current_player->hand.size; i++) {
      char* color = current_card->color;
      char* card[10];
      sprintf(card, "[%s%d],", color, current_card->value);
      strcat(hands, card);
    }
    strcat(hands, "}");
    fputs(hands, input_file);
  }
}

void send_game(game_state game_state) {
  player* current_player = game_state.player_list->head;
  char* data[1000];
  char* size[50];
  for (size_t i = 0; i < game_state.number_players; i++) {
    char* size_value[5];
    sprintf(size_value, "%d,", current_player->hand.size);
    strcat(size, size_value);
    current_player = current_player->next;
  }
  char* top[5];
  sprintf(top, "%s%i", game_state.main.head->color, game_state.main.head->color);
  sprintf(data, "Sizes:%s Top:%s Turn:%i", size, top, game_state.turn);
  for (size_t i = 0; i < game_state.number_players; i++) {
    FILE* input_file = fdopen(current_player->sock_num, "r+");
    fputs(data, input_file);
    current_player = current_player->next;
  }
}

void send_initial(game_state game_state) {
  player* current_player = game_state.player_list->head;
  for (size_t i = 0; i < game_state.number_players; i++) {
    FILE* input_file = fdopen(current_player->sock_num, "r+");
    char* id[5];
    sprintf(id, "%i", current_player->number);
    fputs(id, input_file);
    current_player = current_player->next;
  }
}
