#include "client.h"

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // getline
#include <stdlib.h>      // free
#include <sys/socket.h>  // connect, sockaddr
// #include<stdlib.h/.c>
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
  printf("game receiving \n");
  char* recv_line = NULL;
  printf("about to enter if\n");
  size_t recv_line_size = 0;
  if (getdelim(&recv_line, &recv_line_size, '\0', socket_file) == -1){
    printf("in if\n");
    return -1;
  }
  printf("deserializing\n");

  deserialize(recv_line, game_v);
  free(recv_line);
  return 0;
}

int deserialize(char* recv_line, game_view* game_v){
  printf("%s\n",recv_line);
  if (recv_line[0] == 48){
    char *eptr;
    char *recv_line2 = strdup(recv_line);
    // printf("%i\n", recv_line == NULL);
    //char recv_line1[] = "0/1/1/G4/5/ G3, W1, R2/5/3/45/32/5/3";
    printf("before token declaration\n");
    printf("%s\n", recv_line2);
    char *token = strtok(recv_line2, "/");
    if(token == NULL) {
      printf("here\n");
    }
    printf("%s", token);  
    printf("b4 strtol\n");                    
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
    if (game_v->number_players >= 1) {
        game_v-> player_0 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 2) {
        game_v-> player_1 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 3) {
        game_v-> player_2 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 4) {
        game_v-> player_3 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }if (game_v->number_players >= 5) {
        game_v-> player_4 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    print_game(game_v);

  } else if (recv_line[0] == 49){
    //[1][print until end of line]
    //puts("Broadcast:");
    printf("Broadcast: %s\n",recv_line);
  } else {
    puts("Error doesn't follow format");
  }
  return 0;
}
