#include "client.h"

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // getline
#include <stdlib.h>      // free
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


int receive_hand(FILE* socket_file){
    //send a card name or draw
}

int receive_hand_sizes(FILE* socket_file){
    //get opponents hand size
}

int receive_top_card(FILE* socket_file){
    //find out the top of the deck
}

int receive_game_state(FILE* socket_file){
    //find out whose turn it is or if the game is over
}

int receive_broadcast(FILE* socket_file){
  //find out which player called uno, on who, and what happens
  //if play has been reversed
  //if a wild card has been played
  //if a draw card has been played
  //if a skip has been played
}

int receive_errors(FILE* socket_file){
    //get notified of incorrect moves
}
