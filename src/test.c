#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int message;
    int player_id;
    int turn;
    char *top_card;
    int cards_in_hand;
    char *hand;
    int number_players;
    char *hand_sizes;
    int player_0;
    int player_1;
    int player_2;
    int player_3;
    int player_4;
} game_view;

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


int deserialize(char* recv_line, game_view* game_v){
  printf("%s\n",recv_line);
  if (recv_line[0] == 48){
    char *eptr;

    char recv_line1[] = "0/1/1/G4/5/ G3, W1, R2/5/3/45/32/5/3";

    char *token = strtok(recv_line1, "/");                       
    game_v-> message = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");                           
    game_v-> player_id = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");  
    game_v-> turn = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    strcpy(game_v->top_card, token);
    token = strtok(NULL, "/");
    game_v-> cards_in_hand = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    strcpy(game_v->hand, token);
    token = strtok(NULL, "/");
    game_v-> number_players = strtol(token, &eptr, 10);
    token = strtok(NULL, "/");
    if (game_v->number_players >= 1) {
        game_v-> player_0 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 2) {
        game_v-> player_1 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 3) {
        game_v-> player_2 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }
    if (game_v->number_players >= 4) {
        game_v-> player_3 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }if (game_v->number_players >= 5) {
        game_v-> player_4 = strtol(token, &eptr, 10);
        token = strtok(NULL, "/");
    }


    puts("Struct updated.");
    
    print_game(game_v);

   /*
    printf("%d\n", game_v-> message);
    printf("%d\n", game_v-> player_id);
    printf("%d\n", game_v-> turn);
    printf("%s\n", game_v-> top_card);
    printf("%d\n", game_v-> cards_in_hand);
    printf("%s\n", game_v-> hand);
    printf("%d\n", game_v-> number_players);
    printf("%d\n", game_v-> player_0);
    printf("%d\n", game_v-> player_1);
    printf("%d\n", game_v-> player_2);
    printf("%d\n", game_v-> player_3);
    printf("%d\n", game_v-> player_4);
    */
  } else if (recv_line[0] == 49){
    //[1][print until end of line]
    //puts("Broadcast:");
    printf("Broadcast: %s\n",recv_line);
  } else {
    puts("Error doesn't follow format");
  }
  return 0;
}

int main(void){
    game_view *game_v = malloc(sizeof(game_view));
    game_v->hand = malloc(200); // Space for 200 characters + '\0'
    game_v->top_card = malloc(10); // Space for 10 characters + '\0'
    game_v->hand_sizes = malloc(50); // Space for 50 characters + '\0'

    char* test = "0/1/1/G4/5/G3,W1,R2/5/3,45,32,5";
    char* test1 = "1/This is my broadcast message.";

    deserialize(test, game_v);
}


