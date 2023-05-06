#include "client.h"

#include <stdio.h>   // getline
#include <stdlib.h>  // free
#include <string.h>
#include <sys/socket.h>  // connect, sockaddr

#include "utils.h"

void try_connect(int client_socket, struct sockaddr_in server_addr) {
  int connected = connect(client_socket, (struct sockaddr*)&server_addr,
                          sizeof(server_addr));
  if (connected == -1) {
    error_and_exit("Connecting failed");
  }
}

FILE* get_socket_file(int client_socket) {
  FILE* socket_file = fdopen(client_socket, "w+");
  if (socket_file == NULL) {
    error_and_exit("Couldn't open socket as file stream");
  }
  return socket_file;
}

int send_input(FILE* socket_file) {
  char* send_line = NULL;
  size_t send_line_size = 500;  // NOLINT(*-magic-numbers)
  // put into send line
  printf("about to get line\n");
  if (getline(&send_line, &send_line_size, stdin) == -1) {
    printf("in here :0 \n");
    return -1;
  }
  // put send line into socket file
  printf("putting %s in socket file\n", send_line);
  if (fputs(send_line, socket_file) == EOF) {
    free(send_line);
    error_and_exit("Couldn't send card");
  }
  printf("message sent\n");
  free(send_line);  // now its in socket file
  return 0;
}

int receive_game(FILE* socket_file, game_view* game_v) {
  printf("game receiving \n");
  char* recv_line = NULL;
  printf("about to enter if\n");
  size_t recv_line_size = 0;

  if (getdelim(&recv_line, &recv_line_size, '\0', socket_file) == -1) {
    printf("in if\n");
    return -1;
  }
  // deserialize the input from the server
  deserialize(recv_line, game_v);
  free(recv_line);
  return 0;
}

int deserialize(char* recv_line, game_view* game_v) {
  printf("%s\n", recv_line);
  if (recv_line[0] == forty_eight) {
    // parses the string on the "/"
    char delimeter[2] = "/";
    char* eptr = NULL;
    char* str = strdup(recv_line);
    char* token = NULL;

    // after splitting up string put into the correct struct value
    token = strtok(str, delimeter);  // NOLINT(concurrency-mt-unsafe)
    game_v->message = (int)strtol(token, &eptr, ten);
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    game_v->player_id = (int)strtol(token, &eptr, ten);
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    game_v->turn = (int)strtol(token, &eptr, ten);
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    strcpy(game_v->top_card, token);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    game_v->cards_in_hand = (int)strtol(token, &eptr, ten);
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    strcpy(game_v->hand, token);  // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    game_v->number_players = (int)strtol(token, &eptr, ten);
    token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)

    // checks how many players are in the game before putting them in the struct
    if (game_v->number_players >= 1) {
      game_v->player_0 = (int)strtol(token, &eptr, ten);
      token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    }
    if (game_v->number_players >= 2) {
      game_v->player_1 = (int)strtol(token, &eptr, ten);
      token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    }
    if (game_v->number_players >= 3) {
      game_v->player_2 = (int)strtol(token, &eptr, ten);
      token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    }
    if (game_v->number_players >= 4) {
      game_v->player_3 = (int)strtol(token, &eptr, ten);
      token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    }
    if (game_v->number_players >= five) {
      game_v->player_4 = (int)strtol(token, &eptr, ten);
      token = strtok(NULL, delimeter);  // NOLINT(concurrency-mt-unsafe)
    }
    // call print game from view.c
    print_game(game_v);
    free(str);
    free(token);

  } else if (recv_line[0] == forty_nine) {
    printf("Broadcast: %s\n", recv_line);
  } else {
    puts("Error doesn't follow format");
  }
  return 0;
}
