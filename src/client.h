#pragma once

#include "view.h"

#include <netinet/in.h>  // sockaddr_in
#include <stdio.h>       // FILE



enum{
    /**Magic number error*/
    five = 5,
    forty_eight = 48,
    forty_nine = 49,
    ten = 10
};

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
 * Sends an input from a client to server in file.
 * 
 * It reads from stdin and puts that line into the socket file.
 * 
 * @param socket_file a socket file for a given client.
*/
int send_input(FILE* socket_file);

/**
 * Receives a file with the game state to decode.
 * 
 * Reads from the socket file and passes it to the deserialization function.
 * The deserialization function will print a view.
 * 
 * @param socket_file a socket file for a given client.
 * @param game_v is the game state view struct
*/
int receive_game(FILE* socket_file, game_view* game_v);

/**
 * Reads in a string, splits it into the view struct, and calls print.
 * 
 * If the first value is 0 then it is a game state. If the first value is 1 then
 * it broadcasts message. [0 or 1]/[player_id]/[turn]/[top card]/[# of cards in 
 * hand]/[hand]/[#of players]/[handsize] is the format of a game state.
 * 
 * @param recv_line is a string received from the server
 * @param game_v is the game state view struct
*/
int deserialize(char* recv_line, game_view* game_v);
