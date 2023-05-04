#include "client.h"

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // getline
#include <stdlib.h>      // free
#include <sys/socket.h>  // connect, sockaddr

#include "utils.h"
#include "view.h"

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

int send_input(FILE* socket_file){
  char* send_line = NULL;
  size_t send_line_size = 0;
  //put into send line
  if (getline(&send_line, &send_line_size, stdin) == -1) {
    return -1;
  }
  //put send line into socket file
  if (fputs(send_line, socket_file) == EOF) {
    free(send_line);
    error_and_exit("Couldn't send card");
  }
  free(send_line); //now its in socket file
  return 0;
}


int receive_game(FILE* socket_file, game_view* game_v){
  char* recv_line = NULL;
  size_t recv_line_size = 0;
  if (getline(&recv_line, &recv_line_size, socket_file) == -1) {
    return -1;
  }
  deserialize(recv_line, game_v);
  free(recv_line);
  return 0;
}

int deserialize(char* recv_line, game_view* game_v){
  printf("%s\n",recv_line);
  /*
  if (recv_line[0] == 48){
    char *eptr;
    char *token = strtok(recv_line, "/");                       
    game_v-> message = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");                           
    game_v-> player_id = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");  
    game_v-> turn = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    strcpy(game_v->top_card, token);
    token = strtok(NULL, "/");
    game_v-> cards_in_hand = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    strcpy(game_v->hand, token);
    token = strtok(NULL, "/");
    game_v-> number_players = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    strcpy(game_v->hand_sizes, token);

    printf("%d\n", game_v-> message);
    printf("%d\n", game_v-> player_id);
    printf("%d\n", game_v-> turn);
    printf("%s\n", game_v-> top_card);
    printf("%d\n", game_v-> cards_in_hand);
    printf("%s\n", game_v-> hand);
    printf("%d\n", game_v-> number_players);
    printf("%s\n", game_v-> hand_sizes);
  } else if (recv_line[0] == 49){
    //broadcast
    //[1][print until end of line]
    //[print broadcast]
    puts("broadcast");
  } else {
    //error
    puts("Error doesn't follow format");
  }
  */
  return 0;
}
