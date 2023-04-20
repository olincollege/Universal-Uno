#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

enum { BACKLOG_SIZE = 10 };

// Group the data needed for a server to run.
typedef struct {
  /// The socket descriptor to listen for connections.
  int listener;
  /// The address and port for the listener socket.
  struct sockaddr_in addr;
  /// The maximum number of clients that can be waiting to connect at once.
  int max_backlog;
} uno_server;

/**
 * Create a new echo server in dynamic memory.
 *
 * Given a socket address and a maximum backlog size, create a new echo server
 * on the heap. Since the new server (or rather, the data it stores) is
 * dynamically allocated, the caller is responsible for cleaning the server up
 * afterwards (or terminating the program and letting that take care of things).
 *
 * @param addr The IPv4 address and port that the server will listen on.
 * @param max_backlog The max number of clients that can wait to connect to the
 * server.
 * @return A pointer to the new echo server.
 */
uno_server* make_uno_server(struct sockaddr_in ip_addr, int max_backlog);

/**
 * Free an echo server in dynamic memory.
 *
 * Given a pointer to an echo server on the heap, free the dynamically allocated
 * memory associated with that server. Attempting to free an uninitialized
 * server, server dynamic memory that has already been freed, or a pointer to
 * memory representing anything other than an echo_server instance will result
 * in undefined behavior.
 *
 * @param server A pointer to the server to delete.
 */
void free_uno_server(uno_server* server);

/**
 * Start listening on a server for potential client connections.
 *
 * Bind the listener socket and set it to listen for client connection requests.
 * The listener socket is part of the server's internal data and does not need
 * to be opened separately. In the event that binding or listening on the socket
 * fails, print an error message and exit the program without returning.
 *
 * @param server The server to start listening on.
 */
void listen_for_connections(uno_server* server);

/**
 * Accept a new client connection and start the echo process.
 *
 * Wait until a client connection request comes in, accepting it. Once accepted,
 * fork a new process. In the child process, run the echo process, and in the
 * parent, end the function. The server program should run the function in a
 * loop, whereas the child can exit the function after the echo process. In the
 * event of an error in accepting a connection or forking a new process, print
 * an appropriate error message and accept the program.
 *
 * @param server The server to accept the connection on.
 * @return 0 for the parent process and -1 for the child (echo) process.
 */
int accept_client(uno_server* server);

/**
 * Keeps conversation between clients.
 * 
 * Also waits for the proper amount of clients to join before starting the game
 * 
 * Enter amount of people you want to join.
 * 
 * @param server The server to maintain communication.
*/

void multi_client_communication(uno_server* server, var_game_state game_state);

/**
 * Recieves input from client and updates the state of the game.
*/
void uno(uno_server server, var_game_state game_state);
