#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "view.h"

void print_game(char top_card[], int hand_sizes[], size_t players, int player_turn) {
    printf("Key: B = Blue, R = Red, Y = Yellow, G = Green, W = Wild, +2 = Draw two, W+4 = Wild draw four, r = reverse, S = skip\n");
    printf("Press 'u' to call Uno!\n");
    for (int i = 0; i < (int) players; ++i){
        if (i+1 == player_turn){
            printf("Player %d: %d cards <--Current turn\n", i+1, hand_sizes[i]);
        } else {
            printf("Player %d: %d cards\n", i+1, hand_sizes[i]);
        }
    }
    printf("Deck top card: %s\n", top_card);
}

void print_hand(char hand[], int player_turn, int player_id){
    printf("Hand:[%s]\n", hand);
    if (player_turn == player_id){
        printf("It's your turn! Type out which card you to play or type 'draw'\n");
    } else{
        printf("Wait for the other players to play.\n");
    }
}

void print_error(){
    printf("Error invalid move. Try again.\n");
}

int main(void){
    int test[] = {13, 5, 9};
    char hand[] = "R2, G5, W+4";
    print_game("R2", test, 3, 1);
    print_hand(hand, 1, 2);
}




