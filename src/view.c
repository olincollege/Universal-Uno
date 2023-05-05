#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "view.h"

void print_game(game_view* game_v) {
    printf("Key: B = Blue, R = Red, Y = Yellow, G = Green, W1 = Wild, 2 = Draw two, W4 = Wild draw four, r = reverse, S = skip\n");
    printf("Press 'u' to call Uno!\n");
    if (game_v->number_players >= 1) {
        if (0 == game_v->turn){
            printf("Player 1: %d cards <-- Current turn\n ", game_v-> player_0);
        } else{
            printf("Player 1: %d cards\n", game_v-> player_0);
        }
    }
    if (game_v->number_players >= 2) {
        if (1 == game_v->turn){
            printf("Player 2: %d cards <-- Current turn\n ", game_v-> player_1);
        } else{
            printf("Player 2: %d cards\n", game_v-> player_1);
        }
    }
    if (game_v->number_players >= 3) {
        if (2 == game_v->turn){
            printf("Player 3: %d cards <-- Current turn\n", game_v-> player_2);
        } else{
            printf("Player 3: %d cards\n", game_v-> player_2);
        }
    }
    if (game_v->number_players >= 4) {
        if (3 == game_v->turn){
            printf("Player 4: %d cards <-- Current turn\n", game_v-> player_3);
        } else{
            printf("Player 4: %d cards\n", game_v-> player_3);
        }
    }if (game_v->number_players >= 5) {
        if (2 == game_v->turn){
            printf("Player 5: %d cards <-- Current turn\n", game_v-> player_4);
        } else{
            printf("Player 5: %d cards\n", game_v-> player_4);
        }
    }
    printf("Deck top card: %s\n", game_v->top_card);
    printf("Hand:[%s]\n", game_v->hand);
    if (game_v->turn == game_v->player_id){
        printf("It's your turn! Type out which card you to play or type 'draw'\n");
    } else{
        printf("Wait for the other players to play.\n");
    }
}


// int main(void){
//      int hand_size[] = {3, 7, 1};
//      char hand[] = "R2";
//      print_game("R2", hand_size, 3, 3);
//      print_hand(hand, 1, 2);
// }




