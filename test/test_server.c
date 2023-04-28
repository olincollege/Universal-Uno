#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/server.h"
#include "../src/utils.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

// Check that the listener socket is successfully created.
Test(make_uno_server, listener_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 1111);
  uno_server* server = make_uno_server(addr, BACKLOG_SIZE);
  cr_expect(ge(int, server->listener, 0));
  free_uno_server(server);
}

// Check that the address and port are set correctly.
Test(make_uno_server, addr_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 2222);
  uno_server* server = make_uno_server(addr, BACKLOG_SIZE);
  cr_expect(eq(u32, server->addr.sin_addr.s_addr, htonl(INADDR_ANY)));
  cr_expect(eq(u16, server->addr.sin_port, htons(2222)));
  free_uno_server(server);
}

// Check that the backlog size is set correctly.
Test(make_uno_server, backlog_set_correctly) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 3333);
  uno_server* server = make_uno_server(addr, BACKLOG_SIZE);
  cr_expect(eq(int, server->max_backlog, BACKLOG_SIZE));
  free_uno_server(server);
}

// Check that the socket is accepting connections once it starts listening.
Test(listen_for_connections, socket_listening) {
  struct sockaddr_in addr = socket_address(INADDR_ANY, 4444);
  uno_server* server = make_uno_server(addr, BACKLOG_SIZE);
  listen_for_connections(server);
  int listening = 0;
  socklen_t option_len = sizeof(int);
  cr_expect(ne(int,
               getsockopt(server->listener, SOL_SOCKET, SO_ACCEPTCONN,
                          &listening, &option_len),
               -1));
  cr_expect(ne(int, listening, 0));
  free_uno_server(server);
}

// No tests for accept, as it involves forking behavior that is a bit tough to
// handle in a unit test.

// uno will be tested separately, as it involves setting up a server and
// client.

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
