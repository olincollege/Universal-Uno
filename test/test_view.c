#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "../src/view.h"

// Check that an empty game prints correctly.
Test(print_game, empty_game, .init = cr_redirect_stdout) {
  int hand_size[] = {3};
  print_game("G3", hand_size, 1, 1);
  (void)fflush(stdout);
  (void)fclose(stdout);
  cr_assert_stdout_eq_str(
  "Key: B = Blue, R = Red, Y = Yellow, G = Green, W = Wild, +2 = Draw two, W+4 = Wild draw four, r = reverse, S = skip\n"
  "Press 'u' to call Uno!\n"
  "Deck top card:\n");
}
