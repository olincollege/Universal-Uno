#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "view.h"

void print_board(const struct* board) {
    printf("Deck top card:\n");
    printf("Key: B = Blue, R = Red, Y = Yellow, G = Green, W = Wild, +2 = Draw two, W+4 = Wild draw four, r = reverse, S = skip\n");
    printf("Press 'u' any time to call Uno!\n");
    printf("Players:\n");
    for (int i = 0; i < game_state->players; ++i){
        printf("player %d: %d cards %s\n", i, #_cards, is_it_your_turn);
    }
}

void print_hand(const){
    printf("Hand:[%s]\n", hands);
    if (its_your_turn){
        printf("It's your turn! Type out which card you to play or type 'draw'\n");
    } else{
        printf("Wait for the other players.")
    }
}

void print_error(){
    printf("Error invalid move.\n");
}





