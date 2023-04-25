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
  // Maybe when making players initialize # to the number in the array it is,
  // Deck to NULL and sock_num to NULL
  player* current = game_state.order.head;
  for (int i = 0; i < game_state.number_players) {
    if (current.sock_num != NULL) {
      continue;
    }
    current.sock_num = connected_d;
    break;
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    // Maybe get input from the first player how many people to expect.
    while (game_state.order.head.prev.sock_num == NULL) {
      // Maybe store the current number of players somewhere?
      // View could have a function in the waiting screen to show how many
      // players we are waiting for.
      continue;
    }
    // In the view maybe a while loop that checks if start is != 1 so then when
    // it is we switch the clients view to a screen where cards get passed out.
    if (game_state.start != 1) {
      game_state.start = 1;
      game_state.player_list.cur = game_state.player_list.head;
    }
    uno(game_state, connected_d);
    return -1;
  }

  return 0;
}

void uno(game_state game_state, int socket_descriptor) {
  if (game_state.player_list.head->prev.hand.size < 7) {
    player* current = game_state.player_list.head;
    for (size_t i = 0; i < game_state.number_players; i++) {
      for (size_t i = 0; i < 7; i++) {
        move_card(game_state.draw.head, game_state.draw, current->hand);
        current = current->next;
      }
      // After giving all the players the cards, we will send a message letting,
      // each client know what their cards are.
    }
  }
  FILE* input_file = fdopen(socket_descriptor, "r+");
  char* buf = NULL;
  size_t buf_size = 0;
  getline(&buf, &buf_size, input_file);
  switch (buf[2]) {
    case '0':
      switch_direction(game_state);
      break;
    case '1':
      skip(game_state);
      break;
    case '2':
      draw_two(game_state);
      break;
    case '3':
      // wild card
      break;
    case '4':
      draw_4(game_state);
      break;

    default:
      move_card(current_card, game_state.player_list.cur->hand,
                game_state.main);
      break;
  }
}
