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
  for (int i = 0; i < sizeof(game_state.players) / sizeof(player)) {
    if (game_state.players[i].sock_num != NULL) {
      continue;
    }
    game_state.players[i].sock_num = connected_d;
    break;
  }

  pid_t pid = fork();
  if (pid == -1) {
    error_and_exit("forking problem");
  } else if (pid == 0) {
    // Maybe get input from the first player how many people to expect.
    while (game_state.players[game_state.number_players - 1].sock_num == NULL) {
      // Maybe store the current number of players somewhere?
      // View could have a function in the waiting screen to show how many
      // players we are waiting for.
      continue;
    }
    // In the view maybe a while loop that checks if start is != 1 so then when
    // it is we switch the clients view to a screen where cards get passed out.
    if (game_state.start != 1) {
      game_state.start = 1;
      game_state.turn = 0;
    }
    uno(game_state, connected_d);
    return -1;
  }

  return 0;
}

void uno(game_state game_state, int socket_descriptor) {
  
  FILE* input_file = fdopen(socket_descriptor, "r+");
  char* buf = NULL;
  size_t buf_size = 0;
  getline(&buf, &buf_size, input_file);
  switch (buf[2]) {
    case '0':
      //reverse
      break;
    case '1':
      //skip
      break;
    case '2':
      //draw 2
      break;
    case '3':
    //wild card
      break;
    case '4':
    //draw 4
      break;

    default:
    //regular card
      break;
  }
}
