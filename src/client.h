#pragma once

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE
#include "view.h"

/**
 * Attempt to connect to a server on a given socket.
 *
 * Given an unconnected socket and a server address/port, attempt to connect to
 * a server. If successful, the socket descriptor client_socket can be used to
 * communicate with the server. If not, print an error message and exit without
 * returning from the function.
 *
 * @param client_socket The client's socket descriptor used for the connection.
 * @param server_addr The IPv4 socket address and port of the server.
 */
void try_connect(int client_socket, struct sockaddr_in server_addr);

/**
 * Get a file pointer for a given socket descriptor.
 *
 * Given a socket descriptor used by the client, create and return a file
 * pointer corresponding to that descriptor. This can be used with high-level
 * functions in stdio for convenience. In the event of an error in opening the
 * file stream, print an error message and exit the program, in which case this
 * function does not return.
 *
 * @param client_socket The socket descriptor to generate the file stream for.
 * @return A file pointer corresponding to client_socket.
 */
FILE* get_socket_file(int client_socket);


/**
 * Recieves an input from a client and stores it inside of a file.
 * 
 * @param socket_file a socket file for a given client.
*/
int send_input(FILE* socket_file);

/**
 * Do later
 * 
 * @param socket_file a socket file for a given client.
 * @param game_v
*/
int receive_game(FILE* socket_file, game_view* game_v);

/**
 * Do later
 * 
 * @param recv_line
 * @param game_v
*/
int deserialize(char* recv_line, game_view* game_v);
