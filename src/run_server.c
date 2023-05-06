#include <arpa/inet.h>  // sockaddr_in

#include "server.h"  // uno_server, related functions
#include "utils.h"   // socket_address, PORT

int main(void) {
  struct sockaddr_in server_addr = socket_address(INADDR_ANY, PORT);
  uno_server* server = make_uno_server(server_addr, BACKLOG_SIZE);
  listen_for_connections(server);
  int accept_status = 0;
  game_state* state = make_game_state();
  while (accept_status != -1) {
    printf("accept status %i\n", accept_status);
    accept_status = accept_client(server, state);
  }
  printf("freeing server\n");
  free_uno_server(server);
  return 0;
}
