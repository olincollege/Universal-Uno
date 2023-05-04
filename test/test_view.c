#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "../src/view.h"

// Check that an empty game prints correctly.
Test(print_game, empty_game, .init = cr_redirect_stdout) {
  int hand_size[] = {0};
  print_game("", hand_size, 0, 0);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Key: B = Blue, R = Red, Y = Yellow, G = Green, W = Wild, +2 = Draw two, W+4 = Wild draw four, r = reverse, S = skip\n"
  "Press 'u' to call Uno!\n"
  "Deck top card: \n");
}

// Check that an empty game prints correctly.
Test(print_game, three_player_game_state, .init = cr_redirect_stdout) {
  int hand_size[] = {3, 7, 1};
  print_game("R2", hand_size, 3, 3);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Key: B = Blue, R = Red, Y = Yellow, G = Green, W = Wild, +2 = Draw two, W+4 = Wild draw four, r = reverse, S = skip\n"
  "Press 'u' to call Uno!\n"
  "Player 1: 3 cards\n"
  "Player 2: 7 cards\n"
  "Player 3: 1 cards <--Current turn\n"
  "Deck top card: R2\n");
}

// Check that an empty hand prints correctly.
Test(print_hand, empty_hand, .init = cr_redirect_stdout) {
  char hand[] = "";
  print_hand(hand, 0, 0);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Hand:[]\n"
  "It's your turn! Type out which card you to play or type 'draw'\n"
  );
}

// Check that a hand with cards and its that players turn prints correctly
Test(print_hand, your_turn_full_hand, .init = cr_redirect_stdout) {
  char hand[] = "R2, G3, W+2";
  print_hand(hand, 5, 5);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Hand:[R2, G3, W+2]\n"
  "It's your turn! Type out which card you to play or type 'draw'\n"
  );
}

// Check that a hand with cards and its not that players turn prints correctly
Test(print_hand, not_your_turn_full_hand, .init = cr_redirect_stdout) {
  char hand[] = "R2, G3, W+2";
  print_hand(hand, 5, 3);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Hand:[R2, G3, W+2]\n"
  "Wait for the other players to play.\n"
  );
}
