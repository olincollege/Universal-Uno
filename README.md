# Universal Uno ReadMe:

## Description:
The README describes in sufficient detail the packages needed to compile and run the project.

Universal_UNO is a C implementation of a multiplayer UNO. After setting up the server, up to 5 people can connect to the game and play with each other. The game prints a minimalist visualization of the main card, the number of cards each player has, and your own hand. The visualization will continue updating based on each player and a player wins if they have no cards left. 

## How to play: 
The game follows normal uno rules (https://www.unorules.com/). The player must type out what card they want to play to receive the terminal view. 

## How the program works:


## Dependencies:
To play the game, download all the files from our Universal_UNO repository. You should have the following files:
- .gitignore
- CMakeLists.txt
- README.md
### src
- CMakeLists.txt
- controller.c
- controller.h
- model.c
- model.h
- view.c
- view.h
- server.c
- server.h
- utils.c 
- utils.h
- run_server.c
- run_client.c
### test
- CMakeLists.txt
- test_controller.c
- test_model.c
- test_servr.c
- test_utils.c
- test_view.c

This code only relies on C standard libraries, which means that you won't need to worry about importing any additional libraries.

Instructions:
The README describes in sufficient detail the packages needed to compile and run the project.

To get started, please ensure that you are using a Unix environment to follow the instructions below.
Once you have downloaded the files from the repository, navigate to the Universal_UNO directory. From there, you can create a build directory and compile the program by running the following commands in your terminal:
$ mkdir build
$ cd build
$ cmake ..
$ make clean
$ make

Then run the following command to start the server:
$ ./src/run_server

Each joining  player should follow the same commands as above but run 
$ ./src/run_client. 
Once the total number of clients connects, the game will start!

Have fun playing the game!


Notes:
Currently, our code does not actually run! You can connect 2 clients to the server, and they can receive their hands, however, when you try to send input to the server from the client more than twice, the code segment faults. 
