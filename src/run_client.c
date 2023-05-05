#include <arpa/inet.h>  // INADDR_LOOPBACK, inet_ntop
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>      // fprintf

#include "client.h"
#include "unistd.h"
#include "utils.h"
#include "view.h"

const socklen_t MAX_IP_ADDR_LEN = 16;

int main(void) {
  game_view* game_v = malloc(sizeof(game_view));
  // Space for 200 characters + '\0'
  game_v->hand = malloc(200); // NOLINT(*-magic-numbers)
  // Space for 10 characters + '\0'
  game_v->top_card = malloc(10); // NOLINT(*-magic-numbers)

  // Open a TCP socket to connect to the server.
  int socket_descriptor = open_tcp_socket();
  (void)fprintf(stderr, "Client socket descriptor: %d\n", socket_descriptor);

  // Connect to the server running on this machine (127.0.0.1).
  struct sockaddr_in server_addr = socket_address(INADDR_LOOPBACK, PORT);
  try_connect(socket_descriptor, server_addr);
  // To print an IP address as a string, we need to allocate a buffer.
  char addr_string[MAX_IP_ADDR_LEN];
  (void)fprintf(
      stderr, "Connected to server at %s, port %d\n",
      inet_ntop(AF_INET, &server_addr.sin_addr, addr_string, MAX_IP_ADDR_LEN),
      PORT);

  //Adding a non block flag to file descriptor
  //int flags = fcntl(socket_descriptor, F_GETFL, 0);
  //fcntl(socket_descriptor, F_SETFL, flags | O_NONBLOCK);
  //EAGAIN or the other block one
  //man2 read
  //errno

  // Use a file pointer to make it easier to deal with text lines.
  FILE* socket_file = get_socket_file(socket_descriptor);
  // Echo until either the client or the server closes its stream.
  int socket_file_status = 0;
  int socket_file_status2 = 0;
   while (socket_file_status != -1 && socket_file_status2 != -1) {
    printf("entered while loop\n");
    socket_file_status = send_input(socket_file);
    socket_file_status2 = receive_game(socket_file, game_v);
  }

  // If we didn't hit the end of file for either stdin or the response from the
  // server, then something went wrong.
  // don't hit end of file
  if (!feof(stdin) && !feof(socket_file)) {
    error_and_exit("Error reading or writing line:");
  }

  // Clean up and exit.
  close_tcp_socket(socket_descriptor);
  return 0;
}

