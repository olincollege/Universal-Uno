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
