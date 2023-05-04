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
} game_view;


int deserialize(char* recv_line, game_view* game_v){
  printf("%s\n",recv_line);
  if (recv_line[0] == 48){
    char *eptr;

    //char str[2] = "\0"; /* gives {\0, \0} */
    //str[0] = fgetc(recv_line);

    char recv_line1[] = "1/1/1/G4/5/G3,W1,R2/5/3,45,32,5";

    char *token = strtok(recv_line, "/");                       
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
    strcpy(game_v->hand_sizes, token);

    printf("%d\n", game_v-> message);
    printf("%d\n", game_v-> player_id);
    printf("%d\n", game_v-> turn);
    printf("%s\n", game_v-> top_card);
    printf("%d\n", game_v-> cards_in_hand);
    printf("%s\n", game_v-> hand);
    printf("%d\n", game_v-> number_players);
    printf("%s\n", game_v-> hand_sizes);
  } else if (recv_line[0] == 49){
    //[1][print until end of line]
    puts("Broadcast:");
    printf("%s\n",recv_line);
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
    deserialize(test, game_v);
}


