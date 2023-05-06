#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "../src/view.h"

// NOLINTBEGIN(*-magic-numbers)
// Check that a two player game prints correctly.
Test(print_game, two_playergame, .init = cr_redirect_stdout) {
  game_view* game_v = malloc(sizeof(game_view));
  game_v->player_id = 0;
  game_v->turn = 0;
  game_v->top_card = "G0";
  game_v->cards_in_hand = 1;
  game_v->hand = " R1";
  game_v->number_players = 2;
  game_v->player_0 = 1;
  game_v->player_1 = 1;

  print_game(game_v);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
      "Key: B = Blue, R = Red, Y = Yellow, G = Green, W1 = Wild, 2 = Draw two, "
      "W4 = Wild draw four, r = reverse, S = skip\n"
      "Press 'u' to call Uno!\n"
      "Player 1: 1 cards <-- Current turn\n"
      "Player 2: 1 cards\n"
      "Deck top card: G0\n"
      "Hand:[ R1]\n"
      "It's your turn! Type out which card you to play or type 'draw'\n");
}

// Check that a five player game prints correctly.
Test(print_game, five_playergame, .init = cr_redirect_stdout) {
  game_view* game_v = malloc(sizeof(game_view));
  game_v->player_id = 0;
  game_v->turn = 0;
  game_v->top_card = "G0";
  game_v->cards_in_hand = 1;
  game_v->hand = " R1";
  game_v->number_players = 5;
  game_v->player_0 = 1;
  game_v->player_1 = 1;
  game_v->player_2 = 1;
  game_v->player_3 = 1;
  game_v->player_4 = 1;

  print_game(game_v);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
      "Key: B = Blue, R = Red, Y = Yellow, G = Green, W1 = Wild, 2 = Draw two, "
      "W4 = Wild draw four, r = reverse, S = skip\n"
      "Press 'u' to call Uno!\n"
      "Player 1: 1 cards <-- Current turn\n"
      "Player 2: 1 cards\n"
      "Player 3: 1 cards\n"
      "Player 4: 1 cards\n"
      "Player 5: 1 cards\n"
      "Deck top card: G0\n"
      "Hand:[ R1]\n"
      "It's your turn! Type out which card you to play or type 'draw'\n");
}

// Check that a five player game prints when it's a different players turn.
Test(print_game, five_playergame_diff_turn, .init = cr_redirect_stdout) {
  game_view* game_v = malloc(sizeof(game_view));
  game_v->player_id = 1;
  game_v->turn = 0;
  game_v->top_card = "G0";
  game_v->cards_in_hand = 1;
  game_v->hand = " R1";
  game_v->number_players = 5;
  game_v->player_0 = 1;
  game_v->player_1 = 1;
  game_v->player_2 = 1;
  game_v->player_3 = 1;
  game_v->player_4 = 1;

  print_game(game_v);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
      "Key: B = Blue, R = Red, Y = Yellow, G = Green, W1 = Wild, 2 = Draw two, "
      "W4 = Wild draw four, r = reverse, S = skip\n"
      "Press 'u' to call Uno!\n"
      "Player 1: 1 cards <-- Current turn\n"
      "Player 2: 1 cards\n"
      "Player 3: 1 cards\n"
      "Player 4: 1 cards\n"
      "Player 5: 1 cards\n"
      "Deck top card: G0\n"
      "Hand:[ R1]\n"
      "Wait for the other players to play.\n");
}
// NOLINTEND(*-magic-numbers)
